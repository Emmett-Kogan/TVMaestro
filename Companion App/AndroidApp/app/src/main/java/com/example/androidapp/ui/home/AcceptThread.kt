package com.example.androidapp.ui.home

import android.bluetooth.BluetoothServerSocket
import android.bluetooth.BluetoothSocket
import android.util.Log
import java.io.IOException


// THIS CODE DOES NOT CURRENTLY WORK, MORE RESEARCH IS NEEDED

private class AcceptThread : Thread() {

    //private val mmServerSocket: BluetoothServerSocket? by lazy(LazyThreadSafetyMode.NONE) {
        //bluetoothAdapter?.listenUsingInsecureRfcommWithServiceRecord(NAME, MY_UUID)
    //}

    //override fun run() {
        // Keep listening until exception occurs or a socket is returned.
        //var shouldLoop = true
        //while (shouldLoop) {
            //val socket: BluetoothSocket? = try {
                //mmServerSocket?.accept()
            //} catch (e: IOException) {
                //Log.e(TAG, "Socket's accept() method failed", e)
                //shouldLoop = false
                //null
           // }
           // socket?.also {
                //manageMyConnectedSocket(it)
                //mmServerSocket?.close()
                //shouldLoop = false
            //}
        //}
    //}

    // Closes the connect socket and causes the thread to finish.
    //fun cancel() {
        //try {
            //mmServerSocket?.close()
        //} catch (e: IOException) {
            //Log.e(TAG, "Could not close the connect socket", e)
        //}
    //}
}