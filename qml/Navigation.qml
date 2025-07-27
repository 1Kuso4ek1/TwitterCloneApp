pragma Singleton
import QtQuick

QtObject {
    property var stackView: null

    function push(page, properties) {
        if(stackView)
            stackView.push(page, properties)
    }

    function pop() {
        if(stackView)
            stackView.pop()
    }

    function replace(page, properties) {
        if(stackView)
            stackView.replace(page, properties)
    }

    function clear() {
        if(stackView)
            stackView.clear()
    }
}
