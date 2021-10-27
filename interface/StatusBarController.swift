import AppKit

class StatusBarController {
    private var statusItem: NSStatusItem
    private var statusBarButton: NSStatusBarButton
    
    init(_ menu: NSMenu, _ keyboardMenu: NSMenu) {
        statusItem = NSStatusBar.system.statusItem(withLength: -1)
        statusBarButton = statusItem.button!
        statusBarButton.image = NSImage(named:"keyboard_32")
        statusBarButton.image?.isTemplate = true
        // set the menu
        statusItem.menu = menu
        // create keyboard selection items
        updateKeyboardList(keyboardMenu)
    }
    
    @objc
    func updateKeyboardList(_ keyboardMenu: NSMenu) {
        let keyboardNames = getKeyboardList().takeRetainedValue() as! Array<CFString>
        for name in keyboardNames {
            let value: String = name as String
            keyboardMenu.addItem(NSMenuItem(title:value, action:nil, keyEquivalent:""))
        }
    }
}
