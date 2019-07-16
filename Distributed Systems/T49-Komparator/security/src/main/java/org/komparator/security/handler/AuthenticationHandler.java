package org.komparator.security.handler;

import java.io.ByteArrayInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.security.KeyStoreException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.UnrecoverableKeyException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.util.Iterator;
import java.util.Set;

import javax.xml.bind.DatatypeConverter;
import javax.xml.namespace.QName;
import javax.xml.soap.Name;
import javax.xml.soap.SOAPBody;
import javax.xml.soap.SOAPElement;
import javax.xml.soap.SOAPEnvelope;
import javax.xml.soap.SOAPException;
import javax.xml.soap.SOAPHeader;
import javax.xml.soap.SOAPHeaderElement;
import javax.xml.ws.handler.MessageContext;
import javax.xml.ws.handler.soap.SOAPHandler;
import javax.xml.ws.handler.soap.SOAPMessageContext;

import org.komparator.security.CertUtil;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import pt.ulisboa.tecnico.sdis.ws.cli.CAClient;
import pt.ulisboa.tecnico.sdis.ws.cli.CAClientException;

public class AuthenticationHandler implements SOAPHandler<SOAPMessageContext> {

    public static final String HEADER_NAME = "DigitalSignature";
    public static final String HEADER_NS = "digitalSignature:example";
    String PASSWORD = "2s298rzB";

    public boolean handleMessage(SOAPMessageContext context) {
        Boolean outbound = (Boolean) context.get(MessageContext.MESSAGE_OUTBOUND_PROPERTY);

        if (outbound) {

            try {
                SOAPEnvelope envelope = context.getMessage().getSOAPPart().getEnvelope();
                SOAPHeader header = envelope.getHeader();
                if (header == null)
                    header = envelope.addHeader();

                try {
                    PrivateKey privateKey = CertUtil.getPrivateKeyFromKeyStoreFile("../mediator-ws/src/main/resources/T49_Mediator.jks", PASSWORD.toCharArray(), "t49_mediator", PASSWORD.toCharArray());

                    String envelopeString = envelope.getTextContent();
                    byte[] bytesToSign = DatatypeConverter.parseBase64Binary(envelopeString);
                    byte[] byteSignature = CertUtil.makeDigitalSignature("SHA1WithRSA", privateKey, bytesToSign);
                    String stringSignature = DatatypeConverter.printBase64Binary(byteSignature);

                    Name name = envelope.createName(HEADER_NAME, "e", HEADER_NS);

                    SOAPHeaderElement element = header.addHeaderElement(name);
                    element.addTextNode(stringSignature);
                } catch (UnrecoverableKeyException | FileNotFoundException | KeyStoreException e) {
                    System.out.printf("Failed to create privateKey because of %s%n", e);
                    return false;
                }

            } catch (SOAPException e) {
                System.out.printf("Failed to add SOAP header because of %s%n", e);
                return false;
            }

        } else {
            try {
                SOAPEnvelope envelope = context.getMessage().getSOAPPart().getEnvelope();
                try {
                    CAClient client = new CAClient("http://sec.sd.rnl.tecnico.ulisboa.pt:8081/ca?WSDL");
                    String certificate = client.getCertificate("T49_Mediator");
                    String signature = null;
                    InputStream input = new ByteArrayInputStream(certificate.getBytes());

                    SOAPHeader header = envelope.getHeader();
                    System.out.printf("55555");
                    if (header == null) {
                        System.out.println("Header not found.");
                        return false;
                    }
                    
                    
                    SOAPBody body = envelope.getBody();
                    if (body == null) {
                        System.out.println("Body not found.");
                        return false;
                    }
                    Certificate cert = CertUtil.getX509CertificateFromStream(input);
                    PublicKey publicKey = CertUtil.getPublicKeyFromCertificate(cert);
                    System.out.printf("66666");
                    String envelopeString = envelope.getTextContent();
                    System.out.printf("111111");
                    byte[] bytesToVerify = DatatypeConverter.parseBase64Binary(envelopeString);
                    
                    NodeList childnodes =  envelope.getChildNodes();
                    System.out.printf("22222");
                    for (int i = 0; i <childnodes.getLength(); i++){
					    Node node = childnodes.item(i);
					    System.out.println(node.getNodeName());
					    if(node.getNodeName().equals("DigitalSignature")){
					        signature = node.getTextContent();
					        header.removeChild(node);
					    }
					}
					byte[] byteSignature = DatatypeConverter.parseBase64Binary(signature);
					System.out.printf("3333");
					boolean signatureVerifier = CertUtil.verifyDigitalSignature("SHA1WithRSA", publicKey, bytesToVerify, byteSignature);

					if (!signatureVerifier) {
					    System.out.println("signature not valid");
					    return false;
					}
                    
                } catch (CAClientException e2) {
                    System.out.printf("Failed to get CAClient because of %s%n", e2);
                    return false;
                } catch (CertificateException e) {
                	System.out.printf("Failed to get certificate because of %s%n", e);
                    return false;
				}

            } catch (SOAPException e) {
                System.out.printf("Failed to get SOAP header because of %s%n", e);
                return false;
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