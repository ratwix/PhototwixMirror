import QtQuick 2.0

Item {
    id: choosePhotoControlItem

    Component {
        id: photoPreviewItemDelegate
        PhotoPreviewItem {
            photo:modelData
            height: choosePhotoControlItem.height
        }
    }


    ListView {
        anchors.fill:parent
        id:photoListView
        model : parameters.photoGallery.photoList
        delegate: photoPreviewItemDelegate
        orientation: ListView.Horizontal
        layoutDirection: Qt.LeftToRight
        spacing: 20
    }
}
