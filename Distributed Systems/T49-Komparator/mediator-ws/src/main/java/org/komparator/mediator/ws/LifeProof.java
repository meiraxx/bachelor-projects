package org.komparator.mediator.ws;

import java.time.Duration;
import java.time.LocalDateTime;

import org.komparator.mediator.ws.cli.MediatorClient;

import pt.ulisboa.tecnico.sdis.ws.uddi.UDDINaming;

public class LifeProof extends MyTimerTask {

	private String wsI, uddiURL, wsURL, wsName;
	MediatorPortImpl portImpl = null;

	public static final String SEC_URL = "http://localhost:8072/mediator-ws/endpoint";
	public int flag = 1;
	public UDDINaming uddiNaming = null;

	public LifeProof(MediatorEndpointManager endpoint) {
		super(endpoint);
		this.wsI = endpoint.getWsI();
		this.uddiURL = endpoint.getUddiURL();
		this.wsURL = endpoint.getWsURL();
		this.wsName = endpoint.getWsName();
		this.portImpl = endpoint.getPort();
	}

	public void run() {
		try {
			if (flag == 1) {
				if (wsI.equals("1")) {
					new MediatorClient(SEC_URL).imAlive();
				} else {
					
					Duration duration = Duration.between(portImpl.getTimeStamp(), LocalDateTime.now());

					// se o imAlive nao for recebido
					if (duration.getSeconds() > 5) {
						System.out.println("Primary Server is down");
						System.out.printf("Rebinded '%s' in UDDI to the Secondary Server in %s%n", wsName, uddiURL);
						System.out.println("This server is now the Primary Server");
						uddiNaming = new UDDINaming(uddiURL);
						uddiNaming.rebind(wsName, wsURL);
						flag = 0;
					}
				}
			}
		} catch (Exception e) {
			System.out.println("Waiting for connection from server...");
		}
	}
}

