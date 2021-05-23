import Cocoa
import SwiftUI

@main
class AppDelegate: NSObject, NSApplicationDelegate {
    var aboutWindow: NSWindow!
    var statusBarController: StatusBarController!

    // Outlets
    @IBOutlet weak var menu: NSMenu!
    @IBOutlet weak var keyboardMenu: NSMenu!

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        statusBarController = StatusBarController.init(menu!)
        createAboutWindow()
        readConfiguration()
        if bindInput() < 1 {
            print("error: Failed to capture the input device.")
            print("error:     Either the device is already captured, or input monitoring")
            print("error:     has not been allowed for this executable in 'Security & Privacy'.")
            return
        }
        if bindOutput() < 1 {
            print("error: Failed to connect to the output device.")
            return
        }

        startRunLoop()
    }

    func applicationWillBecomeActive(_ notification: Notification) {
        var ckeyboards: UnsafeMutablePointer<Unmanaged<CFString>?>?
        var count: UnsafeMutablePointer<Int32>?
        getKeyboards(&ckeyboards, &count)

        let keyboards = Array(UnsafeBufferPointer(start: ckeyboards, count: Int(count!.pointee)))

        for i in 0...(Int(count!.pointee)-1) {
            keyboardMenu.addItem(NSMenuItem(
                title: keyboards[i]!.takeUnretainedValue() as String,
                action: nil,
                keyEquivalent: ""))
        }
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }

    func createAboutWindow() {
        aboutWindow = NSWindow(
            contentRect: NSRect(x: 0, y: 0, width: 480, height: 300),
            styleMask: [.titled, .closable, .miniaturizable, .resizable, .fullSizeContentView],
            backing: .buffered, defer: false)
        aboutWindow.isReleasedWhenClosed = false
        aboutWindow.center()
        aboutWindow.setFrameAutosaveName("About")
        aboutWindow.contentView = NSHostingView(rootView: AboutView())
    }
    
    func convertCfTypeToString(cfValue: Unmanaged<CFString>!) -> String?{

        let value = Unmanaged.fromOpaque(
            cfValue.toOpaque()).takeUnretainedValue() as CFString
        if CFGetTypeID(value) == CFStringGetTypeID(){
            return value as String
        } else {
            return nil
        }
    }

    // Actions
    @IBAction func about(sender: AnyObject) {
        aboutWindow.orderFront(nil)
    }
}
