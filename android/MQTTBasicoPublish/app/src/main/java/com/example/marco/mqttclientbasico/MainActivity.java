package com.example.marco.mqttclientbasico;

import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class MainActivity extends AppCompatActivity {


    private static final String TAG = "MAIN";
    String topic        = "casa/marco/1";
    String content      = "1";
    int qos             = 1;
    String broker       = "tcp://10.30.2.127:8883";
    String clientId     = "ClientAndroidBasico43124132";
    String username     = "m";
    String userPass     = "m";
    MemoryPersistence persistence = new MemoryPersistence();
    boolean lampadaStatus = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final ImageView lampada = (ImageView)findViewById(R.id.onoff);

        lampada.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                content = (lampadaStatus? "0": "1");
                lampadaStatus = !lampadaStatus;

                try {
                    MqttClient sampleClient = new MqttClient(broker, clientId, persistence);
                    MqttConnectOptions connOpts = new MqttConnectOptions();
                    connOpts.setUserName(username);
                    connOpts.setPassword(userPass.toCharArray());
                    connOpts.setCleanSession(true);
                    Log.d(TAG, "Conectando ao broker: " + broker);
                    //System.out.println("Connecting to broker: "+broker);
                    sampleClient.connect(connOpts);
                    Log.d(TAG, "Conectado");
                    //System.out.println("Connected");
                    Log.d(TAG, "Enviando mensagem: " + content);
                    //System.out.println("Publishing message: "+content);
                    MqttMessage message = new MqttMessage(content.getBytes());
                    message.setQos(qos);
                    sampleClient.publish(topic, message);
                    Log.d(TAG, "Mensagem envidada");
                    //System.out.println("Message published");
                    sampleClient.disconnect();
                    Log.d(TAG, "Desconectado.");
                    //System.out.println("Disconnected");
                    //System.exit(0);
                } catch(MqttException me) {
                    Log.d(TAG, "reason "+me.getReasonCode());
                    Log.d(TAG, "msg "+me.getMessage());
                    Log.d(TAG, "loc "+me.getLocalizedMessage());
                    Log.d(TAG, "cause "+me.getCause());
                    Log.d(TAG, "excep "+me);
                    me.printStackTrace();
                }
            }
        });

    }
}
