package com.example.marco.mqttbasicosubscribe;

import android.util.Log;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.MqttSecurityException;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

/**
 * Created by Marco on 18/11/2016.
 */

public class ClientSubscribe implements MqttCallback {
    private static final String TAG = "Subscribe";
    MqttClient client;
    String broker       = "tcp://192.168.0.103:8883";
    String clientId     = "ClientAndroidSubscribe";
    String username     = "m";
    String userPass     = "m";
    MemoryPersistence persistence = new MemoryPersistence();
    String topic = "";
    int QoS = 1;

    public ClientSubscribe() {}

    public void Subscribe(String topic, int QoS)
    {
        this.topic = topic;
        this.QoS = QoS;
        try {
            client = new MqttClient(broker, clientId, persistence);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setUserName(username);
            connOpts.setPassword(userPass.toCharArray());
            connOpts.setCleanSession(true);

            client.connect(connOpts);
            Log.d(TAG, "Conectou");

            client.setCallback(this);
            Log.d(TAG, "Call back");

            client.subscribe(topic, QoS);
            Log.d(TAG, "Subscreveu");

        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void connectionLost(Throwable cause) {
        if(!client.isConnected()) {
            try {
                Log.d(TAG, "Conexao perdida.");
                MqttConnectOptions connOpts = new MqttConnectOptions();
                connOpts.setUserName(username);
                connOpts.setPassword(userPass.toCharArray());
                connOpts.setCleanSession(true);

                client.connect(connOpts);
                client.setCallback(this);
                client.subscribe(topic, QoS);

                Log.d(TAG, "Recuperou a conexao");
            } catch (MqttSecurityException e) {
                Log.d(TAG, "Conexao perida: erro 1");
                e.printStackTrace();
            } catch (MqttException e) {
                Log.d(TAG, "Conexao perida: erro 2");
                e.printStackTrace();

            }
        }
    }

    @Override
    public void messageArrived(String topic, MqttMessage message) throws Exception {
        Log.d(TAG, message.toString());
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken token) {
        Log.d(TAG, "Mensagem completa");
    }
}
