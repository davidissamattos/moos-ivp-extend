//
//  ViewController.swift
//  BarcoMOOSApp
//
//  Created by David I. Mattos on 05/10/15.
//  Copyright © 2015 David I. Mattos. All rights reserved.
//

import UIKit
import Alamofire
import SwiftyJSON
import NMSSH

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        let host = "10.0.1.9:22"
        let username = "David"
        let password = "Cheeder"
        let session = NMSSHSession(host: host, andUsername: username)
        print("Trying to connect now..")
        session.connect()
        if session.connected == true
        {
            print("Session connected")
            //Mac requires this kind of authentication
            session.authenticateByKeyboardInteractiveUsingBlock()
            {
             (String) in return password
            }
            
            //For other types
            //session.authenticateByPassword(password)
            if session.authorized == true
            {
                print("Authentication succeeded")
                do
                {
                    
                    let response2 = try session.channel.execute("ls")
                    print(response2)
                }
                catch
                {
                    print("Error ocurred!!")
                }

            }
                    }
        //session.disconnect()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

