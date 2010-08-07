/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.yournamehere.client.sampleservice;

import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 *
 * @author Sean
 */
@RemoteServiceRelativePath("sampleserver/strservice")
public interface strService extends RemoteService {
    public String myMethod(String s);
}
