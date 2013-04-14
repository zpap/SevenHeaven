import bb.cascades 1.0

Page {
    property alias title: mapTitleBar.title
    titleBar: TitleBar {
        id: mapTitleBar
        acceptAction: ActionItem {
            title: qsTr("Refresh")
            onTriggered: {
                mapWebView.reload();
            }
        }
    }
    ScrollView {
        scrollViewProperties.scrollMode: ScrollMode.Both
        scrollViewProperties.pinchToZoomEnabled: true
        Container {
            WebView {
                id: mapWebView
                url: "http://eco-bodhi.herokuapp.com/map.html"
                settings.zoomToFitEnabled: true
            }
        }
    }
}
