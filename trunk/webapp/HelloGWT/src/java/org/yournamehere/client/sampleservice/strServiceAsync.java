/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.yournamehere.client.sampleservice;

import com.google.gwt.user.client.rpc.AsyncCallback;

/**
 *
 * @author Sean
 */
public interface strServiceAsync {
    public void myMethod(String s, AsyncCallback<String> callback);
}
