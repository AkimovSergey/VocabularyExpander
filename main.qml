import QtQuick 2.9
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Window 2.2

Window {
    visible: true
    width: 480
    height: 320
    title: qsTr("VocabularyExpander")
    Rectangle{
              anchors.margins: 10
              color: "transparent"
        GridLayout {

            id: grid
            columns: 1
            rows: 3
            anchors.fill: parent

            TextArea{
                anchors.fill: parent
                id: text_from
            }

            RowLayout{
            Button {
                id: button
            }
            }

            TextArea{
                anchors.fill: parent
                id: text_to
            }

        }
    }

}
