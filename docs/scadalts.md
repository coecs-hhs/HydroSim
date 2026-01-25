# ScadaLTS

**ScadaLTS** will serve as the **HMI (Human-Machine Interface)** for this simulation. It provides capabilities to manipulate the simulation and monitor the status of the water network in real time.

## Automatic Configuration

The ScadaLTS environment needs to be manual imported using a `.zip` configuration file located at:  
`/scadalts/HydroSimV*.zip`, old Configs are located at `scadalts/old_config_zips`

---

# HMI Water Network Overview

## General Overview of the Water Network

The main view of our HMI consists of the SCADA general overview of the water network. In this view, there are also nodes that link to the rest of the network and provide a general overview of how the network looks.

**Figure 1: Overview of the EPANET Water Network**  
![image](https://github.com/user-attachments/assets/bd601fd8-99e8-4c13-a78c-44a215460bb9)

**Figure 2: General Overview**  
![image](img/pid.png)

Our water network was developed from the beginning in an **EPANET file**. In this file, the initial water network was designed, and all values in this document originate from it. It is important to note that both **Figure 1** and **Figure 2** are based on this EPANET file.

---

## Network Structure

The network is divided into two sections:

1. **Zone 0**  
   Water is sent from the water reservoir which we have decided, in this case, to be an infinite water supply to the different networks.  
   ![image](img/Zone0.png)

2. **Zone 1**  
   ![image](img/Zone1.png)

3. **Zone 2**  
   ![image](img/Zone2.png)

4. **Zone 3**  
   ![image](img/Zone3.png)

5. **Zone 4**  
   ![image](img/Zone4.png)

---

# Creating the HMI Screens

The HMI screens that we have created during the 4th and final phase of our project were developed using the software called **PMManager** from Promotic. This is specialized software designed for creating SCADA views for HMIs.

**Link to PMManager:** [PMManager Download](https://www.promotic.eu/en/promotic/download/Pm0900.htm)

**Figure 9: PMManager Software**  
![image](https://github.com/user-attachments/assets/eb1a3a94-774d-4638-a421-e38dcc2cc2fd)

---

## Creating a New Application

After downloading the software, start by creating a new application by clicking the "+" button.

**Figure 10: New Application**  
![image](https://github.com/user-attachments/assets/25602307-064a-44ad-95a6-ace3d9903718)

Once this is done, select "Next" on all options shown. After you’ve selected all options, you will be in your new application. Proceed by going to the **Main Panel Tab**.

---

## Designing the HMI

Here you will be able to add objects and design the HMI to your liking.

**Figure 11: PMManager UI**  
![image](https://github.com/user-attachments/assets/d7f9c102-a4a8-4034-9354-5c4bb8e5e756)

Once you have the application open, the next step is to add "New object" to your HMI. This will allow you to start creating your HMI screens.

**Figure 12: New Objects Added**  
![image](https://github.com/user-attachments/assets/fe9036ac-3e22-4baa-ac2a-84d94b8b4bd2)

---

## Adding Data Resources to Your SCADA Project

Navigate to the **Data Resources** icon in ScadaLTS. Here, you will find a screen where you can add data resources.

**Figure 13: Data Resources in ScadaLTS**  
![image](https://github.com/user-attachments/assets/19c8c4c7-72f2-4637-ad5c-d472d8c29ee9)

These data resources are correlated with the **EPANET file** you have created or any other SCADA-readable file from which you want to take data.

**Figure 14: Adding a New Data Resource**  
![image](https://github.com/user-attachments/assets/513ce54d-7e38-4272-9a2d-d34de4f1e5da)

Fill in the values as shown in figure 15

**Figure 15: Data Resource with Update Period of 1 Second**  
![image](https://github.com/user-attachments/assets/69983bd8-43c9-41b5-ab50-c566817e0f5c)

This resource continuously updates its information and is set to TCP with "Keep Alive." Once these two options are selected, the **Modbus Read Data** section becomes important. Once an offset is filled in, the data registers will be available to be read out.

**Figure 16: Offset Data Being Read Out Starting from 1000**  
![image](https://github.com/user-attachments/assets/9391ec91-632c-488a-981d-c112568473ca)

**Figure 17: EPANET Data File Exported**  
![image](https://github.com/user-attachments/assets/a01c1a9f-429d-44f6-be2c-d134fb706141)

Here we can see the memory addresses associated with different sensors and values that we want to include in the HMI.

---

## Customizing the HMI

Once your file is loaded and your data resources are configured, the only thing left to do is customize your HMI in **ScadaLTS**.

Customizing your HMI can be as simple or as complicated as you wish. A good starting point is to draw an HMI screen in software such as **PMManager**. After you've completed your general HMI screen, you're ready to upload the image to your SCADA views. From here, you can add as many actuators and sensors as you like. Essentially, anything is possible.

**Figure 18: Graphical View for the HMI Screens**  
![image](https://github.com/user-attachments/assets/2e24f212-63a1-4d6f-bb29-42eb80806ed0)

To create a new screen in the graphical view, click on the second icon to the right of the title of your HMI.

**Figure 19: Empty HMI Screen**  
![image](https://github.com/user-attachments/assets/34a4ee25-7598-437b-9fb8-62c47d5a545f)

Now you can upload your HMI image and freely design and add whatever you need. In our project, we mostly used links to different network views and simple data points to read out values from our data resources. We also added buttons to control the valves (turn them on or off).

**Figure 20: Final HMI Screen**  
![image](https://github.com/user-attachments/assets/35791b57-f81a-4047-907b-f133e52079b4)

Here you can upload your image and start adding data points.

**Note** PMManager from Promotic can also be used as the SCADA interface itself. This also makes it so the objects can have animations, which might be interesting. It is however not the most intuitive piece of software to use as a SCADA-interface when it comes to programming it and interacting with it, compared to SCADA-LTS. 

---

## End of the Guide

This concludes the guide for SCADA and how to create your own HMI.

---

