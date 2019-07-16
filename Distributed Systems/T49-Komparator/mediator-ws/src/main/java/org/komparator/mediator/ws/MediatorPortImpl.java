package org.komparator.mediator.ws;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.regex.Pattern;

import javax.jws.HandlerChain;
import javax.jws.WebService;

import org.komparator.mediator.ws.cli.MediatorClient;
import org.komparator.mediator.ws.cli.MediatorClientException;
import org.komparator.supplier.ws.BadProductId_Exception;
import org.komparator.supplier.ws.BadQuantity_Exception;
import org.komparator.supplier.ws.BadText_Exception;
import org.komparator.supplier.ws.InsufficientQuantity_Exception;
import org.komparator.supplier.ws.ProductView;
import org.komparator.supplier.ws.cli.SupplierClient;
import org.komparator.supplier.ws.cli.SupplierClientException;

import pt.ulisboa.tecnico.sdis.ws.cli.CreditCardClient;
import pt.ulisboa.tecnico.sdis.ws.cli.CreditCardClientException;
import pt.ulisboa.tecnico.sdis.ws.uddi.UDDINaming;
import pt.ulisboa.tecnico.sdis.ws.uddi.UDDINamingException;
import pt.ulisboa.tecnico.sdis.ws.uddi.UDDIRecord;

@WebService(endpointInterface = "org.komparator.mediator.ws.MediatorPortType", wsdlLocation = "mediator.wsdl", name = "MediatorWebService", portName = "MediatorPort", targetNamespace = "http://ws.mediator.komparator.org/", serviceName = "MediatorService")

@HandlerChain(file = "/mediator-ws-handler-chain.xml")
public class MediatorPortImpl implements MediatorPortType {

	LocalDateTime timeStamp;
	public static final String SEC_URL = "http://localhost:8072/mediator-ws/endpoint";
	private MediatorEndpointManager endpointManager;
	private int shopID = 0;
	List<CartView> listCarts = new ArrayList<CartView>();
	List<ShoppingResultView> listHistory = new ArrayList<ShoppingResultView>();

	public MediatorEndpointManager getEndpointManager() {
		return endpointManager;
	}

	public LocalDateTime getTimeStamp() {
		return this.timeStamp;
	}

	public void setEndpointManager(MediatorEndpointManager endpointManager) {
		this.endpointManager = endpointManager;
	}

	public MediatorPortImpl(MediatorEndpointManager endpointManager) {
		this.endpointManager = endpointManager;
	}

	/* MAIN METHODS *************************************************************************************************/

	// GET_ITEMS *****************************************************************************************************
	@Override
	public List<ItemView> getItems(String productId) throws InvalidItemId_Exception {

		List<ItemView> itemList = new ArrayList<ItemView>();
		ProductView pview = null;

		if (productId == null || productId.trim().length() == 0) {
			throw new InvalidItemId_Exception(productId, null);
		}

		for (SupplierClient supClient : supplierLookup()) {
			try {
				pview = supClient.getProduct(productId);
			} catch (BadProductId_Exception e) {
				throw new InvalidItemId_Exception(productId, null);
			}

			if (pview == null) {
				continue;
			}

			itemList.add(this.setItem(pview, supClient));
		}

		Collections.sort(itemList, new Comparator<ItemView>() {
			@Override
			public int compare(ItemView item1, ItemView item2) {
				return Integer.valueOf(item1.getPrice()).compareTo(Integer.valueOf(item2.getPrice()));
			}
		});

		if (itemList.size() == 0)
			return null;
		return itemList;
	}

	// SEARCH_ITEMS **************************************************************************************************
	@Override
	public List<ItemView> searchItems(String descText) throws InvalidText_Exception {
		List<ProductView> pviewList = null;
		List<ItemView> itemList = new ArrayList<ItemView>();

		if (descText == null || descText.trim().length() == 0) {
			throw new InvalidText_Exception(descText, null);
		}

		for (SupplierClient supClient : supplierLookup()) {
			try {
				pviewList = supClient.searchProducts(descText);
			} catch (BadText_Exception e) {
				e.printStackTrace();
			}

			if (pviewList == null)
				continue;

			for (ProductView pview : pviewList) {
				if (pview.getDesc().toLowerCase().contains(descText.toLowerCase())) {
					itemList.add(this.setItem(pview, supClient));
				}
			}
		}

		Collections.sort(itemList, new Comparator<ItemView>() {
			@Override
			public int compare(ItemView item1, ItemView item2) {
				String id1 = item1.getItemId().getProductId();
				String id2 = item2.getItemId().getProductId();
				int res = id1.compareTo(id2);
				if (res == 0) {
					int price1 = item1.getPrice();
					int price2 = item2.getPrice();
					res = Integer.compare(price1, price2);
				}
				return res;
			}
		});

		if (itemList.size() == 0)
			return null;
		return itemList;
	}

	// ADD_TO_CART ***************************************************************************************************
	@Override
	public void addToCart(String cartId, ItemIdView itemId, int itemQty) throws InvalidCartId_Exception,
			InvalidItemId_Exception, InvalidQuantity_Exception, NotEnoughItems_Exception {

		CartView cartView;
		UDDINaming uddiNaming = endpointManager.getUddiNaming();

		if (cartId == null || cartId.trim().length() == 0 || Pattern.compile("[^a-zA-Z0-9]").matcher(cartId).find()) {
			throw new InvalidCartId_Exception(cartId, null);
		}
		if (itemId == null || itemId.getProductId() == null || itemId.getProductId().trim().length() == 0
				|| Pattern.compile("[^a-zA-Z0-9]").matcher(itemId.getProductId()).find()) {
			throw new InvalidItemId_Exception(cartId, null);
		}
		if (itemId.getSupplierId() == null || itemId.getSupplierId().trim().length() == 0) {
			throw new InvalidItemId_Exception(cartId, null);
		}
		if (itemQty <= 0) {
			throw new InvalidQuantity_Exception(cartId, null);
		}

		try {
			MediatorClient medCli = new MediatorClient(SEC_URL);
			try {
				String url = uddiNaming.lookup(itemId.getSupplierId());
				SupplierClient supC = null;
				try {
					supC = new SupplierClient(url, itemId.getSupplierId());
				} catch (Exception e2) {
					System.out.println("Can't create SupplierClient");
				}

				for (CartView cart : this.listCarts()) {
					if (cartId.equals(cart.getCartId())) {
						for (CartItemView cartItemView : cart.getItems()) {

							String productId = cartItemView.getItem().getItemId().getProductId();
							String supplierId = cartItemView.getItem().getItemId().getSupplierId();

							if (productId.equals(itemId.getProductId())) {
								if (supplierId.equals(itemId.getSupplierId())) {
									try {
										if (supC.getProduct(productId).getQuantity() < itemQty
												+ cartItemView.getQuantity()) {

											throw new NotEnoughItems_Exception("Insert lower quantity", null);

										} else {
											cartItemView.setQuantity(cartItemView.getQuantity() + itemQty);

											synchronized (listCarts) {
												medCli.updateCart(cart);
											}
											return;
										}
									} catch (BadProductId_Exception e) {
										throw new InvalidItemId_Exception(itemId.getProductId(), null);
									}
								}
							}
						}
						try {
							if (supC.getProduct(itemId.getProductId()).getQuantity() < itemQty) {
								throw new NotEnoughItems_Exception("Insert lower quantity", null);

							} else {
								String id = itemId.getProductId();
								ProductView pview = supC.getProduct(id);
								ItemView itemV = this.setItem(pview, supC);
								CartItemView cartItemView = new CartItemView();
								cartItemView.setQuantity(itemQty);
								cartItemView.setItem(itemV);
								cart.getItems().add(cartItemView);
								synchronized (listCarts) {
									medCli.updateCart(cart);
								}
								return;
							}
						} catch (BadProductId_Exception e) {
							throw new InvalidItemId_Exception(itemId.getProductId(), null);
						}
					}
				}
				cartView = new CartView();
				cartView.setCartId(cartId);

				ProductView pview = null;
				try {
					String id = itemId.getProductId();
					pview = supC.getProduct(id);
					if (pview == null) {
						throw new InvalidItemId_Exception(itemId.getProductId(), null);
					}

					ItemView itemV = this.setItem(pview, supC);
					CartItemView cartItemView = new CartItemView();

					if (pview.getQuantity() < itemQty) {
						throw new NotEnoughItems_Exception("Insert lower quantity", null);
					} else {
						cartItemView.setQuantity(itemQty);
						cartItemView.setItem(itemV);
						cartView.getItems().add(cartItemView);
					}
					synchronized (listCarts) {
						this.listCarts().add(cartView);
						medCli.updateCart(cartView);
					}

				} catch (BadProductId_Exception e1) {
					throw new InvalidItemId_Exception(itemId.getProductId(), null);
				}
			} catch (UDDINamingException e2) {
				e2.printStackTrace();
			}
		} catch (MediatorClientException e3) {
			System.out.println("Can't create MedaitorClient");
		}

	}

	// BUY_CART ******************************************************************************************************
	@Override
	public ShoppingResultView buyCart(String cartId, String creditCardNr)
			throws EmptyCart_Exception, InvalidCartId_Exception, InvalidCreditCard_Exception {
		String url = this.getEndpointManager().getUddiNaming().getUDDIUrl();
		String wsname = this.getEndpointManager().getWsName();
		CreditCardClient cc = null;
		int totalPrice = 0;
		ShoppingResultView shopView = new ShoppingResultView();

		if (cartId == null || cartId.trim().length() == 0) {
			throw new InvalidCartId_Exception(cartId, null);
		}
		CartView c = null;
		for (CartView cart : this.listCarts()) {
			if (cart.getCartId().equals(cartId)) {
				c = cart;
				break;
			}
		}
		if (c == null) {
			throw new InvalidCartId_Exception("CART DOES NOT EXIST", null);
		}
		try {
			cc = new CreditCardClient(url, wsname);
		} catch (CreditCardClientException e) {
			throw new InvalidCreditCard_Exception("Invalid client", null);
		}
		if (creditCardNr == null) {
			throw new InvalidCreditCard_Exception("CC NULL", null);
		}
		if (creditCardNr.trim().length() != 16) {
			throw new InvalidCreditCard_Exception("Invalid credit card", null);
		}
		if (creditCardNr.trim().length() == 0) {
			throw new InvalidCreditCard_Exception("Invalid credit card", null);
		}

		try {
			MediatorClient medCli = new MediatorClient(SEC_URL);

			for (CartView cart : this.listCarts()) {
				if (cartId.equals(cart.getCartId())) {
					if (cart.getItems().isEmpty()) {
						throw new EmptyCart_Exception("This cart is empty", null);
					}
					for (CartItemView cartItemView : cart.getItems()) {
						try {
							String productId = cartItemView.getItem().getItemId().getProductId();
							String supplierId = cartItemView.getItem().getItemId().getSupplierId();
							for (SupplierClient supClient : supplierLookup()) {
								if (supClient.getName().equals(supplierId)) {
									supClient.buyProduct(productId, cartItemView.getQuantity());
									shopView.getPurchasedItems().add(cartItemView);
									totalPrice += cartItemView.getItem().getPrice() * cartItemView.getQuantity();
								}
							}
						} catch (BadProductId_Exception | BadQuantity_Exception | InsufficientQuantity_Exception e) {
							shopView.getDroppedItems().add(cartItemView);
						}
					}
					synchronized (listCarts) {
						medCli.updateCart(cart);
					}
					break;
				}
			}

			shopView.setId(Integer.toString(shopID));
			shopID++;
			if (shopView.getDroppedItems().size() == 0) {
				shopView.setResult(Result.COMPLETE);
			}
			if (shopView.getPurchasedItems().size() == 0) {
				shopView.setResult(Result.EMPTY);
			}
			if (shopView.getDroppedItems().size() != 0 && shopView.getPurchasedItems().size() != 0) {
				shopView.setResult(Result.PARTIAL);
			}
			shopView.setTotalPrice(totalPrice);

			synchronized (listHistory) {
				listHistory.add(shopView);
				medCli.updateShopHistory(shopView);
			}
		} catch (MediatorClientException e1) {
			System.out.println("Can't create MediatorClient");
		}
		return shopView;
	}
	/* AUXILIAR METHODS **********************************************************************************************/

	// PING **********************************************************************************************************
	@Override
	public String ping(String arg0) {
		System.out.println("HELLO FRIEND!!");
		
		String pingResultList = null;

		if (arg0 == null || arg0.trim().length() == 0)
			arg0 = "mediatorName";

		ArrayList<SupplierClient> sups = supplierLookup();

		for (SupplierClient s : sups) {
			try {
				SupplierClient supClient = new SupplierClient(s.getWsURL());
				pingResultList += supClient.ping(supClient.getWsURL());
			} catch (SupplierClientException e) {
				System.err.println("Error!");
			}
		}
		return pingResultList;
	}

	// SUPPLIER_LOOKUP ***********************************************************************************************
	private ArrayList<SupplierClient> supplierLookup() {
		UDDINaming uddiNaming = endpointManager.getUddiNaming();
		ArrayList<SupplierClient> supList = new ArrayList<SupplierClient>();

		Collection<UDDIRecord> urls;
		try {
			urls = uddiNaming.listRecords("T49_Supplier%");
			for (UDDIRecord u : urls) {
				SupplierClient sup = new SupplierClient(u.getUrl(), u.getOrgName());
				supList.add(sup);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return supList;
	}

	// SET ITEM ******************************************************************************************************
	public ItemView setItem(ProductView pview, SupplierClient supClient) {
		ItemIdView itemIdView = new ItemIdView();
		ItemView itemView = new ItemView();

		itemIdView.setProductId(pview.getId());
		itemIdView.setSupplierId(supClient.getName());
		itemView.setItemId(itemIdView);
		itemView.setDesc(pview.getDesc());
		itemView.setPrice(pview.getPrice());

		return itemView;
	}

	// LIST CARS *****************************************************************************************************
	@Override
	public List<CartView> listCarts() {
		return listCarts;
	}

	// CLEAR **********************************************************************************************************
	@Override
	public void clear() {
		for (SupplierClient supClient : supplierLookup()) {
			supClient.clear();
		}
		synchronized (listCarts) {
			listCarts.clear();
		}
		synchronized (listHistory) {
			listHistory.clear();
		}
		shopID = 0;

	}

	// SHOP HISTORY **************************************************************************************************
	@Override
	public List<ShoppingResultView> shopHistory() {
		return listHistory;
	}

	// IM ALIVE ******************************************************************************************************
	public void imAlive() {
		String wsI = this.getEndpointManager().getWsI();

		if (wsI.equals("1")) {
			return;
		} else {
			timeStamp = LocalDateTime.now();
			System.out.println("IS ALIVE");
		}
	}

	// UPDATE CART ****************************************************************************************************
	public void updateCart(CartView cartView) {
		String wsI = this.getEndpointManager().getWsI();

		if (wsI.equals("1")) {
			return;
		} else {
			for (CartView cV : this.listCarts) {
				if (cV.getCartId().equals(cartView.getCartId())) {
					this.listCarts.remove(cV);
					this.listCarts.add(cartView);
					return;
				}
			}
			this.listCarts.add(cartView);
		}

	}


	// UPDATE SHOP HISTORY *********************************************************************************************
	public void updateShopHistory(ShoppingResultView shoppingResultView) {
		String wsI = this.getEndpointManager().getWsI();

		if (wsI.equals("1")) {
			return;
		} else {
			for (ShoppingResultView shopRes : this.listHistory) {
				if (shopRes.getId().equals(shoppingResultView.getId())) {
					this.listHistory.remove(shopRes);
					this.listHistory.add(shopRes);
					return;
				}
			}
			this.listHistory.add(shoppingResultView);
		}

	}

}
