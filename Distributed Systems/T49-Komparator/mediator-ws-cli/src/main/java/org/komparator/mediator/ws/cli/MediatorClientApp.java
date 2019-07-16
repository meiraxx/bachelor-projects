package org.komparator.mediator.ws.cli;

import java.util.List;
import java.util.Scanner;

import org.komparator.mediator.ws.ItemIdView;
import org.komparator.mediator.ws.ItemView;
import org.komparator.mediator.ws.ShoppingResultView;
import org.komparator.supplier.ws.cli.SupplierClient;


public class MediatorClientApp {

    public static void main(String[] args) throws Exception {
        // Check arguments
        if (args.length == 0) {
            System.err.println("Argument(s) missing!");
            System.err.println("Usage: java " + MediatorClientApp.class.getName()
                    + " wsURL OR uddiURL wsName");
            return;
        }
        String uddiURL = null;
        String wsName = null;
        String wsURL = null;
        if (args.length == 1) {
            wsURL = args[0];
        } else if (args.length >= 2) {
            uddiURL = args[0];
            wsName = args[1];
        }

        // Create client
        MediatorClient client = null;
        SupplierClient supplierClient = null;
        final String VALID_CC = "1234567890123452";
        ItemIdView id = null;
        
        if (wsURL != null) {
            System.out.printf("Creating client for server at %s%n", wsURL);
            client = new MediatorClient(wsURL);
            supplierClient = new SupplierClient(wsURL);
        } else if (uddiURL != null) {
            System.out.printf("Creating client using UDDI at %s for server with name %s%n",
                uddiURL, wsName);
            client = new MediatorClient(uddiURL, wsName);
            supplierClient = new SupplierClient(uddiURL, wsName);
        }

        // the following remote invocations are just basic examples
        // the actual tests are made using JUnit
        while(true){
        	System.out.println();
        	System.out.println("************* Main menu ************");
            System.out.println();
	        System.out.println("************************************");
	        System.out.println("* Pick a number:	           *");
	        System.out.println("************************************");
	        System.out.println("* (1)Ping			   *");
	        System.out.println("* (2)GetItems                      *");
	        System.out.println("* (3)SearchItems                   *");
	        System.out.println("* (4)AddToCart                     *");
	        System.out.println("* (5)BuyCart                       *");
	        System.out.println("************************************");
	
	        
	        Scanner scanner = new Scanner(System.in);
	        short number = scanner.nextShort();
	        
	        client.uddiLookup();
            client.createStub();
	        	
	        switch (number) {
	            case 1: System.out.println("Invoke ping()...");
			            client.ping("client");
	                    break;
	                    
	        
	            case 2: System.out.println("Getting item 'p1'...");
			            List<ItemView> result2 = client.getItems("p1");
			            System.out.println(result2);
			            break;
		            
	                    
	            case 3: System.out.println("Searching item with description: 'AAA'");
			            List<ItemView> result3 = client.searchItems("AAA");
			            System.out.println(result3);
		                break;
	            	
	                    
	            case 4: System.out.println("Adding 11 items 'p1' to 'Cart1'");
	            		id = new ItemIdView();
	            		id.setProductId("p1");
	            		id.setSupplierId(supplierClient.getName());
			            client.addToCart("Cart1", id, 11);
			            System.out.println("Item added to Cart1");
		                break;
	            
	            case 5: System.out.println("Buying the whole 'Cart1'");
	            		ShoppingResultView result5 = client.buyCart("xyz", VALID_CC);
			            System.out.println(result5);
			            break;
	            default :
	            		break;
	        } 
        
        }

    }
}