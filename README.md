# Homeware
This API create an integration between hardware devices like ESP8266 or Arduino and the Google Assitant using Firebase for the backend.

<img alt="Super Grover" src="https://github.com/kikeelectronico/homeware/raw/master/images/cloud.png" />

## Supported Devices

| Device | Status |
| ------ | ------ |
| Light | Supported |
| Blind | Supported |
| Thermostat | In progress |

## To Do

- Update database from hardware (temperature, buttons, ...)

## Deploying the API

This is important.
- Follow this steps one by one and in order.
- Be patient
- Use the same Google account in all steps

### Activity Controls

Go to <a href="https://myaccount.google.com/activitycontrols" target="blanck">Activity Control</a> by Google and enable:

- Web & App Activity
- Device Iformation
- Voice & Audio Activity


<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B1C1.png" width="400"/>
</kbd>
<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B1C2.png" width="400"/>  <img src="https://github.com/kikeelectronico/homeware/raw/master/images/B1C3.png" width="400"/>
</kbd>

### Actions on Google Project

Go to the  <a href="https://console.actions.google.com/" target="blanck">Actions on Google console</a>.

1. Select `Add/import project`.
2. Write a name for the project.

<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B2C2.png"/>
</kbd>

3. Select `Home control`.

<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B2C3.png"/>
</kbd>

4. Select `Smart home`.

<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B2C4.png"/>
</kbd>

5. Go to `Invocation` under `Setup` in the left side menu.

6. Write `Home` as Display Name and click `Save`.

7. Go to `Backend service` under `Advanced Options` in the left side menu.

8. Click in `View in Firebase`.

<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B2C5.png"/>
</kbd>

9. Click in the gear icon in the left side menu and select `Project Configuration`.

<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B2C6.png"/>
</kbd>

10. Take note of the `Project ID` for later.


Great, now we have a new project. We will be back to the project later.


### Enable HomeGraph API

1. Go to <a href="https://console.cloud.google.com/apis/api/homegraph.googleapis.com/overview">Google Cloud Platform</a>, select your project if it is not selected and enable the HomeGraph API.
2. Select `Credentials` under `APIs & Services` from the left side menu.
3. Click `Create Credentials`.
4. Select `API key`.

Take note of the new API key, we will use it later.

### Firebase CLI

1. We need to install CLI on our machine.
   CLI uses npm wich comes with NodeJS, so make sure that you have this toolchain installed before run the following command.

```Markdown
npm -g install firebase-tools
```
 2. Lets verify the installation.

 ```Markdown
 firebase --version
 ```
3. Login with your Google account

```Markdown
firebase login
```

4. Download or clone this repository.

5. Make sure you are in the API directory and run:

```Markdown
firebase use --add
```

4. Select your Project ID and follow the instructions.

5. Go to the `functions` directory and install the dependencies:

```Markdown
npm install
```

6. Create the following enviroment variables:

```Markdown
firebase functions:config:set system.api-key="[API_key]"

firebase functions:config:set system.api-uri="https://us-central1-[id].cloudfunctions.net"

```
- [API_key] is the Google Cloud API Key (You get it from Enable HomeGraph API).
- [id] the project ID from Firebase (You get it in step 10 from Actions on Google Project).

7. Go back to `API` directory and deploy the API:

```Markdown
firebase deploy
```

8. Check the hosting URL. You will get something like `https://[id].firebaseapp.com` in the console. Go to this URL with your web browser and you will see a Hello message.


### Real time database structure

1. Go to <a href="https://console.firebase.google.com" target="blanck">Firebase Console</a>.
2. Select `Database` under the `Develop` menu.
3. Create a new `Realtime Database`.

<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B4C1.png"/>
</kbd>

4. Enable both read and write operations.

<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B4C2.png"/>
</kbd>

5. Go to the 3 dots menu and select `Import JSON`.

6. Upload the JSON file located inside the `Database` directory.



### Configure the project in Actions on Google

1. Select the <a href="https://console.firebase.google.com" target="blanck">Firebase console</a> and select the project.

2. Select `Actions` under `Build` section in the left side menu.

3. Add the fullfillment URL using your project ID (You get it in step 10 from Actions on Google Project).

```Markdown
https://us-central1-[id].cloudfunctions.net/smarthome
```
4. Select `Done`.

5. Go to `Account Linking` under `Advenced Options` in the left side menu.

6. Select `No, I only want to allow account creation on my website` and clic `Next`.

<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B5C1.png"/>
</kbd>

7. Select `OAuth` and `Authorization Code`, clic `Next`.

<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B5C2.png"/>
</kbd>

8. Enter the Client Information as follow and click `Next`:

<kbd>
<img src="https://github.com/kikeelectronico/homeware/raw/master/images/B5C3.png"/>
</kbd>


| Parameter | Value |
| --- | --- |
| Client ID | 123 |
| Client Secret | 456 |
| Authorization URL | https://us-central1-[id].cloudfunctions.net/fakeauth |
| Token URL | https://us-central1-[id].cloudfunctions.net/faketoken |

9. Enable `Google to transmit clientID and secret via HTTP basic auth header` in Configure your Client by clicking 'Next'.

10. Write the following in testing instructions and click `Save`.

```Markdown
No authentication needed
```
11. Go to `Simulator` under `Test` section in the left side menu.

12. Select `START TESTING`.

### Link your Account

Use your smartphone to link the project.

1. Open Google Home App.

2. Select `Add`.

3. Select `New device`.

4. Select `Works with Google`.

5. Select the options that looks like:

```Markdown
[test] Home
```

Up & running

Now we should see a light called Bulb in our Google Home App.

### Config cron jobs

There are some jobs that can be done using a Cron Task.

1. Go to <a href="https://console.cloud.google.com/cloudscheduler">Google Cloud Platform</a>.

2. Enable `CLoud Scheduler` tool.

3. Create a new Task:

| Parameter | Value |
| --------- | ----- |
| Name | Give it a name |
| Description | Give it a description |
| Frecuency | * * * * * |
| Time zone | Your time zone |
| Destiny | HTTP |
| URL | https://us-central1-[id].cloudfunctions.net/cron |
| Method | GET |
| Header | None |

## Check if the API is working

1. On your smarthome, open the Google Home App.

2. Find `Bulb` device.

3. On the Firebase Realtime Database, find `light` unde `status`.

4. Change the Bulb status from the App, it should change in the database

Note: If you open the Bulb in the Home App and the Bulb it is not online (there isn't any hardware bulb) the app will say `Not responding` and the `online` will change to false in the database. This is normal, it is part of the API.


## Connecting an ESP8266

You can use the ESP8266 example to communicate with the API using this hardware.

***

Any idea will be well received!

***

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Licencia Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">Homeware</span> by <span xmlns:cc="http://creativecommons.org/ns#" property="cc:attributionName">Enrique Gómez</span> licensed by <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.<br /><br />
