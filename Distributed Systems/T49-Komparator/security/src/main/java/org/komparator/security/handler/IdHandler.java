package org.komparator.security.handler;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.Set;
import java.util.UUID;

import javax.xml.namespace.QName;
import javax.xml.soap.Name;
import javax.xml.soap.SOAPElement;
import javax.xml.soap.SOAPEnvelope;
import javax.xml.soap.SOAPException;
import javax.xml.soap.SOAPHeader;
import javax.xml.soap.SOAPHeaderElement;
import javax.xml.ws.handler.MessageContext;
import javax.xml.ws.handler.soap.SOAPHandler;
import javax.xml.ws.handler.soap.SOAPMessageContext;

public class IdHandler implements SOAPHandler<SOAPMessageContext> {

	public static final String HEADER_NAME = "MsgID";
	public static final String HEADER_NS = "id:example";
	public static final String ID = UUID.randomUUID().toString();

	public boolean handleMessage(SOAPMessageContext context) {
		Boolean outbound = (Boolean) context.get(MessageContext.MESSAGE_OUTBOUND_PROPERTY);
		ArrayList<String> idList = new ArrayList<String>();
		
		if (outbound) {

			try {
				SOAPEnvelope envelope = context.getMessage().getSOAPPart().getEnvelope();

				SOAPHeader header = envelope.getHeader();
				if (header == null)
					header = envelope.addHeader();

				Name name = envelope.createName(HEADER_NAME, "e", HEADER_NS);
				SOAPHeaderElement element = header.addHeaderElement(name);
				
				String uuid = UUID.randomUUID().toString();
				element.addTextNode(uuid);

			} catch (SOAPException e) {
				System.out.printf("Failed to add SOAP header because of %s%n", e);
			}

		} else {

			try {
				SOAPEnvelope envelope = context.getMessage().getSOAPPart().getEnvelope();

				SOAPHeader header = envelope.getHeader();
				if (header == null) {
					System.out.println("Header not found.");
					return true;
				}

				Name name = envelope.createName(HEADER_NAME, "e", HEADER_NS);
				Iterator it = header.getChildElements(name);

				if (!it.hasNext()) {
					System.out.printf("Header element %s not found.%n", HEADER_NAME);
					return true;
				}
				
				SOAPElement element = (SOAPElement) it.next();
				
				String headerValue = element.getValue();
				
				if(idList.size() == 0){
					idList.add(headerValue);
				}
				else{
					for(String s : idList){
						if(s == headerValue){
							throw new RuntimeException();
						}
					}
				}
				
				

			} catch (SOAPException e) {
				System.out.printf("Failed to get SOAP header because of %s%n", e);
			}
		}
		return true;
	}



	public boolean handleFault(SOAPMessageContext context) {
		return true;
	}

	public Set<QName> getHeaders() {
		return null;
	}

	public void close(MessageContext messageContext) {
	}

}
