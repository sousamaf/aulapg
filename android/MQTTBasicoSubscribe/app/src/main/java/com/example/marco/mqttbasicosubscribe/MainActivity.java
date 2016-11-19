package com.example.marco.mqttbasicosubscribe;

import android.app.Activity;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView texto = (TextView)findViewById(R.id.texto);
        new ClientSubscribe().Subscribe("casa/lampada/+/status", 1);
    }
    public void setTexto(String t)
    {
        TextView texto = (TextView)findViewById(R.id.texto);
        texto.setText(t);
    }
}
