import Cocoa
import SwiftUI

@main
class AppDelegate: NSObject, NSApplicationDelegate {
    @IBOutlet weak var menu: NSMenu!
    @IBOutlet weak var keyboardMenu: NSMenu!
    
    var aboutWindow: NSWindow!
    var statusBarController: StatusBarController!
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        createAboutWindow()
        createHIDManager()
        statusBarController = StatusBarController.init(self, menu, keyboardMenu)
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
    
    @IBAction func about(sender: AnyObject) {
        aboutWindow.orderFront(nil)
    }
    
    func startCapture(_ productID: Int32, _ vendorID: Int32) {
        releaseInput()
        releaseOutput()
        stopRunLoop()
        readConfiguration()
        if bindInput(productID, vendorID) < 1 {
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
}
