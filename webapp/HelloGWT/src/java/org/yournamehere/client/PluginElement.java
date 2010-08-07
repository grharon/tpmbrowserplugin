/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.yournamehere.client;

import com.google.gwt.dom.client.Element;

/**
 *
 * @author Sean
 */
public class PluginElement extends Element{
    protected PluginElement() {
    }

    public final native String getKeyword() /*-{
        return this.keyword;
    }-*/;

    public final native void setKeyword(String value) /*-{
        this.keyword = value;
    }-*/;

    public final native String callGetURL()/*-{
        return this.GetURL();
    }-*/;
}
