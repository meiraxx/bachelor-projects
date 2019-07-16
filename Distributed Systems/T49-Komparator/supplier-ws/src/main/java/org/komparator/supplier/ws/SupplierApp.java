package org.komparator.supplier.ws;


/** Main class that starts the Supplier Web Service. */
public class SupplierApp {
	private String _uddiURL;
	private String _wsName;
	
	public SupplierApp(String uddiURL, String wsName){
		this._uddiURL= uddiURL;
		this._wsName = wsName;
	}
	
	public static void main(String[] args) throws Exception {
		// Check arguments
		if (args.length < 1) {
			System.err.println("Argument(s) missing!");
			System.err.println("Usage: java " + SupplierApp.class.getName() + " wsURL");
			return;
		}
		String wsURL = args[0];
		String _uddiURL = args[1];
		String _wsName = args[2];
		
		// Create server implementation object
		SupplierEndpointManager endpoint = new SupplierEndpointManager(wsURL, _uddiURL, _wsName);
		
		try {
			endpoint.start();
			endpoint.awaitConnections();
		} finally {
			endpoint.stop();
		}

	}

}
