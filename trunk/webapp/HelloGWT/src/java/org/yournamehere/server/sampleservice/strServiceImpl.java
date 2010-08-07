/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.yournamehere.server.sampleservice;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;

import org.yournamehere.client.sampleservice.strService;

/**
 *
 * @author Sean
 */
public class strServiceImpl extends RemoteServiceServlet implements strService {
    public String myMethod(String s) {
        // Do something interesting with 's' here on the server.
        return "Server says: " + s+" and "+s;
    }
}
