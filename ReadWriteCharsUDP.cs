
using System;
using System.Threading;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Video;

using System.IO.Ports;

public class ReadWriteCharsUDP : MonoBehaviour
{
    [Header("UDP Info")]    
    public UDPSender sender;
    public UDPReceiver receiver;
    
    string receivedString = "";

    [Header("Videos")]
    public VideoPlayer yellowLightTrail;
    public VideoPlayer lightBlueLightTrail;

    void Awake() {
        receiver.onDataReceived += ReadUDP; 
    }

    void Update() {
        // Change cube color based on received serial character 'a' or 'b':
        

        // Send characters 'c' and 'd' if respective keys are pressed:
        if (Input.GetKeyDown(KeyCode.A)) {
            SendCharacter('a');
            PlayVideo(yellowLightTrail);
        }
        if (Input.GetKeyDown(KeyCode.B)) {
            SendCharacter('b');
            PlayVideo(lightBlueLightTrail);
        }
    }

    void SendCharacter(char sentChar) {
        sender.sendMessage(""+sentChar);
        Debug.Log("Sent character: " + sentChar);
    }

     void PlayVideo(VideoPlayer vp) {
        yellowLightTrail.Stop();
        lightBlueLightTrail.Stop();
        vp.Play();
    }

    private void ReadUDP(string s) { //Runs on an infinite loop in another thread. Do not call manually elsewhere!
        receivedString = s;
    }
}
