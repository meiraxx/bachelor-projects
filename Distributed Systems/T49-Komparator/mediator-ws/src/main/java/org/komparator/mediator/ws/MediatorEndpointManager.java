package org.komparator.mediator.ws;

import java.io.IOException;

import javax.xml.ws.Endpoint;

import pt.ulisboa.tecnico.sdis.ws.uddi.UDDINaming;

/** End point manager */
public class MediatorEndpointManager {
	
	int serverUp = 0;
	/** UDDI naming server location */
	private String uddiURL;
	/** Web Service name */
	private String wsName;
	
	private String wsI;
	
	/** Web Service location to publish */
	private String wsURL;
	

	/** Get Web Service UDDI publication name */
	public String getWsName() {
		return wsName;
	}
	
	public String getWsI() {
		return wsI;
	}
	
	public String getUddiURL() {
		return uddiURL;
	}
	
	public String getWsURL() {
		return wsURL;
	}
	
	/** Get UDDI Naming instance for contacting UDDI server */
	UDDINaming getUddiNaming() {
		return uddiNaming;
	}


	private MediatorPortImpl portImpl;


	public MediatorPortImpl getPort() {
        return portImpl;
	}

	/** Web Service endpoint */
	private Endpoint endpoint = null;
	/** UDDI Naming instance for contacting UDDI server */
	private UDDINaming uddiNaming = null;

	/** output option **/
	private boolean verbose = true;

	public boolean isVerbose() {
		return verbose;
	}

	public void setVerbose(boolean verbose) {
		this.verbose = verbose;
	}

	/** constructor with provided UDDI location, WS name, and WS URL */
	public MediatorEndpointManager(String uddiURL, String wsName, String wsURL, String wsI) {
		this.uddiURL = uddiURL;
		this.wsName = wsName;
		this.wsURL = wsURL;
		this.wsI = wsI;
	}

	/** constructor with provided web service URL */
	public MediatorEndpointManager(String wsURL) {
		if (wsURL == null)
			throw new NullPointerException("Web Service URL cannot be null!");
		this.wsURL = wsURL;
	}

	/* end point management */

	public void start() throws Exception {
		try {
			portImpl = new MediatorPortImpl(this);
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
		if(wsI.equals("2"))
			unpublishFromUDDI();
	}

	/* UDDI */

	void publishToUDDI() throws Exception {
		try {
			// publish to UDDI
			if (!(wsI.equals("1"))){
				uddiNaming = new UDDINaming(uddiURL);
				System.out.printf("Secondary Server '%s'%n", wsName);
			}
			else {
				if (uddiURL != null ) {
					if (verbose) {
						System.out.printf("Publishing Primary Server '%s' to UDDI at %s%n", wsName, uddiURL);
					}
					uddiNaming = new UDDINaming(uddiURL);
					uddiNaming.rebind(wsName, wsURL);
				}
			}
		} catch (Exception e) {
			uddiNaming = null;
			if (verbose) {
				System.out.printf("Caught exception when binding to UDDI: %s%n", e);
			}
			throw e;
		}
	}

	void unpublishFromUDDI() {
		try {
			if(!wsI.equals("1")){
				if (uddiNaming != null) {
					// delete from UDDI
					uddiNaming.unbind(wsName);
					if (verbose) {
						System.out.printf("Unpublished '%s' from UDDI%n", wsName);
					}
					uddiNaming = null;
				}
			}
		} catch (Exception e) {
			if (verbose) {
				System.out.printf("Caught exception when unbinding: %s%n", e);
			}
		}
	}

}