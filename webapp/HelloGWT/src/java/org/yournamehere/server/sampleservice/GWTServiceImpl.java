/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.yournamehere.server.sampleservice;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import org.yournamehere.client.sampleservice.GWTService;

/**
 *
 * @author Sean
 */
public class GWTServiceImpl extends RemoteServiceServlet implements GWTService {
    private Random randomizer = new Random();
    private static final long serialVersionUID=-15020842597334403L;
    private static List quotes =new ArrayList();
    static {
        quotes.add("No great thing is created suddenly - Epictetus");
        quotes.add("Well done is better than well said - Ben Franklin");
        quotes.add("No wind favors he who has no destined port - Montaigne");
        quotes.add("Sometimes even to live is an act of courage - Seneca");
        quotes.add("Know thyself - Socrates");
    }
    public String myMethod() {
        return (String) quotes.get(randomizer.nextInt(5));
    }
}
