package org.komparator.security.handler;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Set;

import javax.xml.namespace.QName;
import javax.xml.soap.SOAPException;
import javax.xml.soap.SOAPMessage;
import javax.xml.ws.handler.MessageContext;
import javax.xml.ws.handler.soap.SOAPHandler;
import javax.xml.ws.handler.soap.SOAPMessageContext;

public class LoggingHandler implements SOAPHandler<SOAPMessageContext> {

	@Override
	public Set<QName> getHeaders() {
		return null;
	}

	@Override
	public boolean handleMessage(SOAPMessageContext smc) {
		logToSystemOut(smc);
		return true;
	}

	@Override
	public boolean handleFault(SOAPMessageContext smc) {
		logToSystemOut(smc);
		return true;
	}

	@Override
	public void close(MessageContext messageContext) {
	}

	private SimpleDateFormat dateFormatter = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS");

	private void logToSystemOut(SOAPMessageContext smc) {
		Boolean outbound = (Boolean) smc.get(MessageContext.MESSAGE_OUTBOUND_PROPERTY);

		System.out.print("[");
		System.out.print(dateFormatter.format(new Date()));
		System.out.print("] ");
		
		System.out.print("intercepted ");
		if (outbound)
			System.out.print("OUTbound");
		else
			System.out.print(" INbound");
		System.out.println(" SOAP message:");

		SOAPMessage message = smc.getMessage();
		try {
			message.writeTo(System.out);
			System.out.println(); // add a newline after message

		} catch (SOAPException se) {
			System.out.print("Ignoring SOAPException in handler: ");
			System.out.println(se);
		} catch (IOException ioe) {
			System.out.print("Ignoring IOException in handler: ");
			System.out.println(ioe);
		}
	}

}
