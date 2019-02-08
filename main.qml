import QtQuick 2.9
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Window 2.2

Window {
    id: window
    visible: true
    width: 480
    height: 320
    title: qsTr("VocabularyExpander")

    Column{
        id: column
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        TextArea {
            id: ta_from

        }

    Row{
    Button {
        id: bt_check
        text: qsTr("Check")
    }
    Button {
        Layout.rightMargin: 3
        id: bt_tip
        text: qsTr("Tip")
    }
    }

    TextArea {
        id: ta_to
    }
    }

}

