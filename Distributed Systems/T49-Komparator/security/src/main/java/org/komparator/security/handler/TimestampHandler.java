package org.komparator.security.handler;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Iterator;
import java.util.Set;

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

public class TimestampHandler implements SOAPHandler<SOAPMessageContext> {

	public static final String HEADER_NAME = "TimeStamp";
	public static final String HEADER_NS = "urn:example";

	@Override
	public boolean handleMessage(SOAPMessageContext context) {
			Boolean outbound = (Boolean) context.get(MessageContext.MESSAGE_OUTBOUND_PROPERTY);
			if (outbound) {

				try {
					SOAPEnvelope envelope = context.getMessage().getSOAPPart().getEnvelope();

					SOAPHeader header = envelope.getHeader();
					if (header == null)
						header = envelope.addHeader();

					Name name = envelope.createName(HEADER_NAME, "e", HEADER_NS);
					SOAPHeaderElement element = header.addHeaderElement(name);
					
					element.addTextNode(LocalDateTime.now().toString());

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

					
					DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd'T'HH:mm:ss.SSS");
					
					LocalDateTime oldDate = LocalDateTime.parse(headerValue, formatter);
					LocalDateTime newDate = oldDate.plusSeconds(3);
					LocalDateTime actualDate = LocalDateTime.now();

					
					if(!actualDate.isBefore(newDate)){
						throw new RuntimeException();
					}

				} catch (SOAPException e) {
					System.out.printf("Failed to get SOAP header because of %s%n", e);
				}
			}
			return true;
		}

	@Override
	public boolean handleFault(SOAPMessageContext context) {
		return true;
	}

	@Override
	public void close(MessageContext context) {
	}

	@Override
	public Set<QName> getHeaders() {
		return null;
	}

}
