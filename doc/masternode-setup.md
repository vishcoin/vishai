This guide is designed to create a secure masternode.  That means the coins you have will stay safe in your local wallet (i.e. on your pc wallet) and the masternode that will be conencted to by others will be on a different machine (i.e. a VPS such as Vultr, use Vultr with this link and we both get free credit https://www.vultr.com/?ref=8233173 ).

<b><u>Installing Dependeincies for vishai Coin on Your VPS</u></b><br><br>

Before you can run a masternode you have to be able to run the Vishai Coin daemon/wallet.  This requires the installation of several dependenicies.  To install these run the following commands on your VPS:<br><br>

        sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils

        sudo apt-get install libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev

        sudo add-apt-repository ppa:bitcoin/bitcoin
        sudo apt-get update
        sudo apt-get install libdb4.8-dev libdb4.8++-dev

        sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler

<br><br>
<b><u>Preparing Collateral in Local Wallet</u></b>

1. Create an address in your local wallet by:<br>
    a. QT/GUI Wallet: selecting the Recieve tab, entering a label (name) and pressing the "Request Payment" button<br><br>
    b. Command line: entering command ./vishai-cli getnewaddress<br><br>
The address will be presented back to you, copy it for use in the next steps

2. Send your local wallet 1000000 VISH by:<br>  
    a. QT/GUI Wallet: selecting the Send tab, entering the address obtained from step 1 in the "Pay To:" field and entering "1000000" in the "Amount" field.  Make sure the "Subtract fee from amount" check box is <b>NOT</b> checked.  You will see the transaction on the screen in the Overview and Transactions tab<br><br>
    b. Command line: entering command ./vishai-cli sendtoaddress "address from step 1" 1000000.  The system will give you the TransID as confirmation <br>


3. Now wait for your transaction to have 15 confirmations so the network has been properly updated.  Do this by:
    a. QT/GUI Wallet: Hovering your mouse over the transaction which will be called "payment to yourself".  Don't worry, it won't show 1000000 Vishai coin, only the fee. The wallet will pop up a message such as (6 confirmations, will be available after 10).  You are wating on this to reach 15 confirmations.<br><br>
    b. Command line: entering command ./vishai-cli listtransactions.  The system will give an output similar to the below, you are waiting on the confirmations to reach 15.<br>

                "account": "",
                    "address": "WXQ2sBjp1bNEqrtiCG934Cd2ES5Egtgf4J",
                    "category": "received",
                    "amount": 1000000.00000000,
                    "vout": 1,
                    "fee": -0.00020582,
                    <b>"confirmations": 6,</b>
                    "instantlock": false,
                    "blockhash": "000004d63c47db2ded3ae015e9e267b28dc64aab48beb768f28c1cb78884dbcd",
                    "blockindex": 1,
                    "blocktime": 1581371506,
                    "txid": "ddd852619c0a0146da4cb3179d6c1181a7a198ef268051680616f532c997fd05",
                    "walletconflicts": [
                    ],
                    "time": 1581371506,
                    "timereceived": 1582910655,
                    "bip125-replaceable": "no",
                    "abandoned": false
                  }
<br>
<b><u>Obtaining Keys in Local Wallet</u></b><br><br>

Running a masternode requires 2 keys, an output and a private key to identify the node on the network.  Both are obtained from the local wallet by:<br><br>

a. QT/GUI Wallet: Open the debig console by clicking "Tools" then clicking "Debug Console". In the ext entry space on the bottom of the popup windows enter the following 2 commands and save the long string the system will give you for use them in steps:<br><br>
        masternode outputs<br>
        masternode genkey<br><br>

Now remove the quotes and the colon from the result of the masternode outputs command for use in the next steps. The end result is a long string and number at the end.  The number will be a 1 or 0 <br><br>
b. Command line: enter the following 2 commands and save the long string the system will give you for use them in steps:<br>
            ./vishai-cli masternode outputs<br>
            ./vishai-cli masternode genkey<br><br> 
            
Now remove the quotes and the colon from the result of the masternode outputs command for use in the next steps. The end result is a long string and number at the end.  The number will be a 1 or 0

<br><br>

<b><u>Opening the proper Port on Firewall</u></b><br><br>

It is necessary for other clients to talk to your masternode for it to be rewarded and cosidered Enabled.  
Open the firewall on your VPS by entering command:<br> 

ufw allow 33333
<br><br>
If you have your linux server behind a router you will have to forward port 33333 to your linux server
<br><br>

<b><u>Gathering Additional Information Needed for .conf Files</u></b><br><br>
You will need to obtain you public IP address as well as decide on a user name and password for your masternode. Accomplish this by:<br><br>
a. Access your VPS and type command
<br><br>
ip addr
<br><br>
You will receive an output similar to the following:<br><br>

            ens3: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP group default qlen 1000
                link/ether 56:00:02:31:fa:7c brd ff:ff:ff:ff:ff:ff
                inet <b>49.12.117.64</b>/22 brd 45.32.219.255 scope global ens3<br>
                   valid_lft forever preferred_lft forever
                inet6 fe80::5400:2ff:fe31:fa7c/64 scope link
                   valid_lft forever preferred_lft forever
<br><br>
The bold area is your ip address.  Save it somewhere for use in the next steps
<br><br>
Next you need to think of a rpcuser (user name) and rpcpassword (password) to use.  these can be anything, just save them off for the next steps.  Good examples are:<br><br>
    
    rpcuser=vishaiproawesomeuser123123
    rpcpassword=878765ghjhg675ytfytufgklhlk
    
<br><br>

<b><u>Preparing the conf Files</u></b><br><br>
Now you are going to update 2 files ending in .conf on your local machine that you sent the 1000000 vishai to as well as create 2 .conf files on the VPS/Linux Server.  These are located in the data directory your wallet is using. When installing the wallet you would have chosen the location but can also access the files by using the buttons in the GUI/QT wallet by:<br><br>

a. QT/GUI Wallet: Click the Tools menu then click "Open Masternode Configuration File" which will open a text file.  In the text file you will paste in the following configuration substituting in the information captured in previous steps.<br><br>

mn your_ip_address:33333 your_masternode_genkey your_masternode_output<br><br>

This will look similar to the following:<br><br>

        mn 49.12.117.64:33333 7Wowe2ZWhjddh1FXxi5UqBqSvGNkx3NsYKY4NLnhStZEJYmMyqg  7422b6e19b55ae41cb153b9be83d50721dfg6fd78a365763a7d35d2840967d54 0 

<br><br>
b. VPS via Command line: In the termial of your VPS create the folder and the file with the following command:<br><br>

mkdir ~/.vishaicore && nano ~/.vishaicore/masternode.conf<br><br>

Now paste in the following configuration substituting in the information captured in previous steps. It may be easier to do this in a text editor on your local machine so you can paste it all in at once.  <br><br>

        mn your_ip_address:33333 your_masternode_genkey your_masternode_output
<br><br>
This will look similar to the following:<br><br>

        mn 49.12.117.64:33333 7Wowe2ZWhjddh1FXxi5UqBqSvGNkx3NsYKY4NLnhStZEJYmMyqg  7422b6e19b55ae41cb153b9be83d50721dfg6fd78a365763a7d35d2840967d54 0 

<br><br>
Save your file.  To save in nano press control and x at the same time, press y to confirm, press enter save<br><br>

The second file you will create is named vishai.conf and will only be done on the VPS.  Create the file in nano by entering the following command:<br><br>

        nano ~/.vishaicore/vishai.conf

<br><br>
Add the following substituting in the info you gathered above<br><br>

        listen=1
        server=1
        daemon=1
        rpcuser=your_rpc_user
        rpcpassword=your_rpcpassword
        rpcallowip=127.0.0.1
        masternode=1
        masternodeprivkey=your_masternode_genkey
        externalip=your_ip_address:33333
    
<br><br>
Save your file.  To save in nano press control and x at the same time, press y to confirm, press enter save<br><br>

It is now time to download and install the Vishai Coin daemon to run on your VPS/Linux Server.  enter the following commands to download and extract the daemon on your VPS.  If you decide to do this in a different folder other than your home folder you need to kconsider that when starting it alter.  This guide assumes you are putting in in your home folder and running as the root user that most VPS providers give you.  First navigate to the Vishai Coin github at https://github.com/vishcoin/vishai/releases and find the release that matches the VPS operating system you arerunning.  This guide is running it on ubuntu 18.04 so substitute in the url that best matching your system:<br><br>

        wget https://github.com/vishcoin/vishai/releases/download/v2.0.0.2/vishai-v2002-ubuntu18-64.zip
        unzip vishai-v2002-ubuntu18-64.zip
        chmod vishaid vishai-cl vishai-tx 755

<br><br>
Now its time to start the daemon up, do this by entering the following command:
<br><br>

./vishaid
<br><br>

When you start the daemon you must wait until it's fully in sync by entering command:
<br><br>
        ./vishai-cli mnsync status

The output will look similar to the following:<br><br>

          "AssetID": 999,
          "AssetName": "MASTERNODE_SYNC_FINISHED",
          "AssetStartTime": 1582306115,
          "Attempt": 0,
          "IsBlockchainSynced": true,
          "IsMasternodeListSynced": true,
          "IsWinnersListSynced": true,
          <b>"IsSynced": true,</b>
          "IsFailed": false
<br><br>
When the "IsSynced" row say true you are ready to start the node up by using command: <br><br>

        ./vishai-cli masternode start-alias mn
 <br><br>       

The "mn" in the command is the name you gave your node.  Feel free to call it anything you like.

You can check status of your masternode by using command: <br><br>

        ./vishai-cli masternode status
<br><br>
If you did everything right you will see "Successfully Started" in the message given

Now its time to set up Sentinel to keep things running smoothly and avoid WATCHDOG_EXPIRED status.  This is not required as your node will still receive paymenrs but still looks better.  Visit https://github.com/vishaipro/sentinel/blob/master/README.md to continue for Linux and https://github.com/vishaipro/sentinel/blob/master/sentinel-for-windows.md for Windows


