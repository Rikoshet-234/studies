import sys
import re
import shelve
from gi.repository import Gtk
from datetime import datetime


class AddressBook(Gtk.Window):
    
    def __init__(self):
        super(AddressBook, self).__init__(title="Address Book")

        # Contacts list.
        store = Gtk.ListStore(str)
        self.selector = Gtk.TreeView(store)
        self.selection = self.selector.get_selection()
        self.selection.connect('changed', self.on_selection_changed)

        cell = Gtk.CellRendererText()        
        renderer = Gtk.CellRendererText()
        column = Gtk.TreeViewColumn("Contacts", renderer, text=0)
        self.selector.append_column(column)

        # Contact information.
        self.nameLabel = Gtk.Label(label="Name:")
        self.nameEntry = Gtk.Entry()

        self.phoneLabel = Gtk.Label(label="Phone:")
        self.phoneEntry = Gtk.Entry()

        self.emailLabel = Gtk.Label(label="E-mail:")
        self.emailEntry = Gtk.Entry()

        self.addressLabel = Gtk.Label(label="Address:")
        self.addressEntry = Gtk.Entry()

        self.checkoutLabel = Gtk.Label(label="Last checkout:")
        self.checkoutEntry = Gtk.Entry(editable=False)

        self.saveButton = Gtk.Button(label="Save contact")
        self.saveButton.connect('clicked', self.on_save_clicked)

        self.addButton = Gtk.Button(label="Add contact")
        self.addButton.connect('clicked', self.on_add_clicked)

        self.removeButton = Gtk.Button(label="Remove contact")
        self.removeButton.connect('clicked', self.on_remove_clicked)

        self.filterLabel = Gtk.Label(label="Phone filter")
        self.filterEntry = Gtk.Entry()
        self.filterEntry.connect('changed', lambda _: self.update_contacts())

        # Set layout.
        hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=10)

        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=5)
        vbox.pack_start(self.filterLabel, False, False, 0)
        vbox.pack_start(self.filterEntry, False, False, 0)
        vbox.pack_start(self.selector, True, True, 0)
        vbox.pack_start(self.addButton, False, False, 0)
        vbox.pack_start(self.removeButton, False, False, 0)

        hbox.pack_start(vbox, True, False, 0)

        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=5)
        vbox.pack_start(self.nameLabel, True, False, 0)
        vbox.pack_start(self.nameEntry, True, False, 0)
        vbox.pack_start(self.phoneLabel, True, False, 0)
        vbox.pack_start(self.phoneEntry, True, False, 0)
        vbox.pack_start(self.emailLabel, True, False, 0)
        vbox.pack_start(self.emailEntry, True, False, 0)
        vbox.pack_start(self.addressLabel, True, False, 0)
        vbox.pack_start(self.addressEntry, True, False, 0)
        vbox.pack_start(self.checkoutLabel, True, False, 0)
        vbox.pack_start(self.checkoutEntry, True, False, 0)
        vbox.pack_start(self.saveButton, True, False, 0)
        
        hbox.pack_start(vbox, True, False, 0)

        self.add(hbox)

        # Load database.
        self.db = shelve.open("data.db")
        self.update_contacts()


    def update_contacts(self):
        self.db.sync()

        try:
            pattern = re.compile(self.filterEntry.get_text())
        except:
            return
        
        store = self.selector.get_model()
        store.clear()

        for name in self.db:
            if pattern.search(self.db[name]["phone"]):
                store.append([name])


    def on_save_clicked(self, button):
        model, treeit = self.selection.get_selected()
        if not treeit:
            return

        name = model[treeit][0]

        # First, delete old record (convinient if name changes).
        del self.db[name]
        # Now, just add record with new values.
        self.db[self.nameEntry.get_text()] = {
            "phone": self.phoneEntry.get_text(),
            "email": self.emailEntry.get_text(),
            "address": self.addressEntry.get_text(),
            "checkout": str(datetime.now())
        }

        self.update_contacts()


    def on_add_clicked(self, button):
        self.db["Unknown"] = {
            "phone": "unknown",
            "email": "unknown@example.com",
            "address": "Nowhere 0/0",
            "checkout": "never"
        }

        self.update_contacts()
    

    def on_remove_clicked(self, button):
        model, treeit = self.selection.get_selected()
        if not treeit:
            return

        del self.db[model[treeit][0]]
        self.update_contacts()


    def on_selection_changed(self, selection):
        model, treeit = self.selection.get_selected()
        if not treeit:
            return

        name = model[treeit][0]
        record = self.db[name]

        self.nameEntry.set_text(name)
        self.phoneEntry.set_text(record["phone"])
        self.emailEntry.set_text(record["email"])
        self.addressEntry.set_text(record["address"])
        self.checkoutEntry.set_text(record["checkout"])


nab = AddressBook()
nab.connect('delete-event', Gtk.main_quit)
nab.show_all()
Gtk.main()
