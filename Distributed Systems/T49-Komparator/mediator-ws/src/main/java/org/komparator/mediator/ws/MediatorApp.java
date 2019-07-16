package org.komparator.mediator.ws;

import java.util.Timer;

public class MediatorApp {

	public static void main(String[] args) throws Exception {
		// Check arguments
		if (args.length == 0 || args.length == 2) {
			System.err.println("Argument(s) missing!");
			System.err.println("Usage: java " + MediatorApp.class.getName() + " wsURL OR uddiURL wsName wsURL");
			return;
		}
		String uddiURL = null;
		String wsName = null;
		String wsURL = null;
		String wsI = null;
		
		// Create server implementation object, according to options
		MediatorEndpointManager endpoint = null;
		if (args.length == 1) {
			wsURL = args[0];
			endpoint = new MediatorEndpointManager(wsURL);
		} else if (args.length >= 3) {
			uddiURL = args[0];
			wsName = args[1];
			wsURL = args[2];
			wsI = args[3];
			endpoint = new MediatorEndpointManager(uddiURL, wsName, wsURL, wsI);
			endpoint.setVerbose(true);
		}

		try {
			endpoint.start();
			LifeProof lifeProof = new LifeProof(endpoint);
			if (lifeProof.flag == 1){
				new Timer().scheduleAtFixedRate(lifeProof, 5*1000, 5*1000);
			}
			endpoint.awaitConnections();
		} finally {
			endpoint.stop();
		}

	}

}
