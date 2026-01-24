import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 900
    height: 600
    title: "HydroSim GUI"

    // -------------------------
    // Current image index
    // -------------------------
    property int currentImageIndex: 0

    // -------------------------
    // List of all available images (backgrounds)
    // -------------------------
    property var images: [
        { name: "Main Page", src: "qrc:/images/Main Page.png" },
        { name: "Epanet Overview", src: "qrc:/images/Epanet Overview.png" },
        { name: "Zone 0", src: "qrc:/images/Zone0.png" },
        { name: "Zone 1", src: "qrc:/images/Zone1.png" },
        { name: "Zone 2", src: "qrc:/images/Zone2.png" },
        { name: "Zone 3", src: "qrc:/images/Zone3.png" },
        { name: "Zone 4", src: "qrc:/images/Zone4.png" }
    ]

    // -------------------------
    // Background image
    // -------------------------
    Image {
        id: bg
        anchors.fill: parent
        source: images[currentImageIndex].src
        fillMode: Image.PreserveAspectFit

        // -------------------------
        // Create buttons dynamically for each device
        // -------------------------
        Repeater {
            model: backend.getDevices()  // Get devices from C++ backend

            delegate: Button {
                parent: bg

                // Show button only on the corresponding zone image
                visible: (modelData.zone + 2) === currentImageIndex

                // -------------------------
                // Button appearance
                // -------------------------
                text: modelData.status ? "Turn Off" : "Turn On"
                background: Rectangle {
                    color: modelData.status ? "#B71C1C" : "#229b26"  // red = on, green = off
                    radius: 2
                }

                // -------------------------
                // Button position relative to image
                // -------------------------
                x: (bg.width - bg.paintedWidth) / 2 + bg.paintedWidth * modelData.position.x
                y: (bg.height - bg.paintedHeight) / 2 + bg.paintedHeight * modelData.position.y

                // -------------------------
                // Button size relative to image
                // -------------------------
                width: bg.paintedWidth * 0.096
                height: bg.paintedHeight * 0.054

                // -------------------------
                // Button logic: toggle device state
                // -------------------------
                onClicked: {
                    backend.toggleDevice(modelData)
                }

                // -------------------------
                // Listen for backend state changes (not needed in current version, could be usefull for future additions)
                // -------------------------
                // Connections {
                //     target: backend
                //     function onButtonStateChanged(device, state) {   // pass Device* from C++
                //         if (device === modelData) {
                //             // Update your delegate (button) directly
                //             modelData.setStatus(state)   // if you expose a setStatus() Q_INVOKABLE
                //         }
                //     }
                // }
            }
        }
    }

    // -------------------------
    // Dropdown menu to select image
    // -------------------------
    ComboBox {
        id: imageSelector
        model: images
        textRole: "name"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        // Update current image index when dropdown changes
        onCurrentIndexChanged: currentImageIndex = currentIndex
    }
}
