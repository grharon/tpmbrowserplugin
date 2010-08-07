/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.yournamehere.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.core.client.GWT;
import com.google.gwt.dom.client.Document;
import com.google.gwt.dom.client.Element;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.PasswordTextBox;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.TextBox;
import org.yournamehere.client.sampleservice.strService;
import org.yournamehere.client.sampleservice.strServiceAsync;

/**
 * Main entry point.
 *
 * @author Sean
 */
public class MainEntryPoint implements EntryPoint {
    /** 
     * Creates a new instance of MainEntryPoint
     */
    public MainEntryPoint() {
    }

    /** 
     * The entry point method, called automatically by loading a module
     * that declares an impslementing class as an entry-point
     */
    public void onModuleLoad() {
        final Button submitButton = new Button("submit");
        final Button callButton = new Button("call");
        final TextBox nameField = new TextBox();
        final PasswordTextBox pwdField = new PasswordTextBox();
	final PasswordTextBox TPMField = new PasswordTextBox();
        final Label strLabel = new Label();
        final PluginElement embed1 = (PluginElement) Document.get().getElementById("embed1");

        nameField.setText("user");
        pwdField.setText("user");
        TPMField.setText("123456");
        RootPanel.get().add(strLabel);
        RootPanel.get("nameFieldContainer").add(nameField);
        RootPanel.get("pwdFieldContainer").add(pwdField);
	RootPanel.get("TPMFieldContainer").add(TPMField);
	RootPanel.get("sendButtonContainer").add(submitButton);
        RootPanel.get("sendButtonContainer").add(callButton);

        nameField.setFocus(true);
        nameField.selectAll();

        class MyAsyncCallBack implements AsyncCallback {
            public void onFailure(Throwable arg0) {
                String text = strLabel.getText();
                strLabel.setText(text+"Failed to get a string.");
            }
            public void onSuccess(Object result) {
                String text = strLabel.getText();
                strLabel.setText(text+(String) result);
            }
        }
        final MyAsyncCallBack myACB = new MyAsyncCallBack();

        submitButton.addClickHandler(new ClickHandler() {

            public void onClick(ClickEvent event) {
                String name = nameField.getText();
                String pwd = pwdField.getText();
                String tpm = TPMField.getText();
                String news = "";
                if (embed1 != null)
                    //embed1.setKeyword(tpm+tpm);
                    news = embed1.getKeyword();
                getStrService().myMethod("NAME= "+name + "PWD= "+pwd+"PLUGIN= "+news, myACB);
            }
        });

        callButton.addClickHandler(new ClickHandler() {

            public void onClick(ClickEvent event) {
                if (embed1 != null)
                    embed1.callGetURL();
                else
                    throw new UnsupportedOperationException("Not supported yet.");
            }
        });
    }

    public static strServiceAsync getStrService() {
        return GWT.create(strService.class);
    }

}
