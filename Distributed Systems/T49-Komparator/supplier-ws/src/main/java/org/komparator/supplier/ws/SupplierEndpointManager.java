package org.komparator.supplier.ws;

import java.io.IOException;

import javax.xml.ws.Endpoint;

import pt.ulisboa.tecnico.sdis.ws.uddi.UDDINaming;


/** End point manager */
public class SupplierEndpointManager {
	
	private String _uddiURL = null;
	private String _wsName = null;
	private UDDINaming uddiNaming = null;

	/** Web Service location to publish */
	private String wsURL = null;

	/** Port implementation */
	private SupplierPortImpl portImpl = new SupplierPortImpl(this);

// TODO
//	/** Obtain Port implementation */
//	public SupplierPortType getPort() {
//		return portImpl;
//	}

	/** Web Service end point */
	private Endpoint endpoint = null;

	/** output option **/
	private boolean verbose = true;

	public boolean isVerbose() {
		return verbose;
	}

	public void setVerbose(boolean verbose) {
		this.verbose = verbose;
	}

	public SupplierEndpointManager(String wsURL, String uddiURL, String wsName) {
		if (wsURL == null)
			throw new NullPointerException("Web Service URL cannot be null!");
		this.wsURL = wsURL;
		
		if (uddiURL == null)
			throw new NullPointerException("UDDI URL cannot be null!");
		this._uddiURL= uddiURL;
		
		if (wsName == null)
			throw new NullPointerException("Web Service Name cannot be null!");
		this._wsName = wsName;
	}

	/* end point management */

	public void start() throws Exception {
		try {
			// publish end point
			endpoint = Endpoint.create(this.portImpl);
			if (verbose) {
				System.out.printf("Starting %s%n", wsURL);
			}
			endpoint.publish(wsURL);
		} catch (Exception e) {
			endpoint = null;
			if (verbose) {
				System.out.printf("Caught exception when starting: %s%n", e);
				e.printStackTrace();
			}
			throw e;
		}
		publishToUDDI();
	}

	public void awaitConnections() {
		if (verbose) {
			System.out.println("Awaiting connections");
			System.out.println("Press enter to shutdown");
		}
		try {
			System.in.read();
		} catch (IOException e) {
			if (verbose) {
				System.out.printf("Caught i/o exception when awaiting requests: %s%n", e);
			}
		}
	}

	public void stop() throws Exception {
		try {
			if (endpoint != null) {
				// stop end point
				endpoint.stop();
				if (verbose) {
					System.out.printf("Stopped %s%n", wsURL);
				}
			}
		} catch (Exception e) {
			if (verbose) {
				System.out.printf("Caught exception when stopping: %s%n", e);
			}
		}
		this.portImpl = null;
		unpublishFromUDDI();
	}
	
	void publishToUDDI() throws Exception {
		try {
			// publish to UDDI
			if (_uddiURL != null) {
				if (verbose) {
					System.out.printf("Publishing '%s' to UDDI at %s%n", _wsName, _uddiURL);
				}
				uddiNaming = new UDDINaming(_uddiURL);
				uddiNaming.rebind(_wsName, wsURL);
			}
		} catch (Exception e) {
			uddiNaming = null;
			if (verbose) {
				System.out.printf("Caught exception when binding to UDDI: %s%n", e);
			}
			throw e;
		}
	}

	
	
	
	void unpublishFromUDDI() throws Exception{
		try {
			if (uddiNaming != null) {
				// delete from UDDI
				uddiNaming.unbind(_wsName);
				if (verbose) {
					System.out.printf("Unpublished '%s' from UDDI%n", _wsName);
				}
				uddiNaming = null;
			}
		} catch (Exception e) {
			if (verbose) {
				System.out.printf("Caught exception when unbinding: %s%n", e);
			}
		}
	}

}
