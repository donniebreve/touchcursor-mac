import AppKit

class StatusBarController {
    private var statusItem: NSStatusItem
    private var statusBarButton: NSStatusBarButton
    
    init(_ menu: NSMenu) {
        statusItem = NSStatusBar.system.statusItem(withLength: -1)
        statusItem.menu = menu
        statusBarButton = statusItem.button!
        statusBarButton.image = NSImage(named:"keyboard_32")
        statusBarButton.image?.isTemplate = true
    }
}
