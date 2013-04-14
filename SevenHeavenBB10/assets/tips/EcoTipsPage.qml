import bb.cascades 1.0
import bb.data 1.0
import eco.bodhi 1.0

Page {
    property alias title: tipsTitleBar.title
    titleBar: TitleBar {
        id: tipsTitleBar
    }
    Container {
        ListView {
            id: tipsList
            dataModel: SimpleDataController { id: tipsDataModel }
            listItemComponents: [
                ListItemComponent {
                    Container {
                        TipItem {}
                    }
                }
            ]
        }
        onCreationCompleted: {
            ecoBodhi.loadData.connect(loadDataModels);
            loadDataModels();
        }
        
        function loadDataModels() {
            // populate library list view model with all shows from library
            tipsDataModel.load("data/feeds/tips.json");
        }
    }
}
