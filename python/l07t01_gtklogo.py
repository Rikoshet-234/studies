import sys
import cairo
import math
from gi.repository import Gtk


WIDTH = 400
HEIGHT = 400


class Vector:
    """Turtle movement vector, no need for usual vector operators."""

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def rotate(self, angle):
        # Stolen from somewhere.
        radians = math.radians(angle)
        cos = math.cos(radians)
        sin = math.sin(radians)

        x = self.x * cos - self.y * sin
        y = self.x * sin + self.y * cos
        
        self.x = x
        self.y = y


class Turtle:
    """Contains all possible turtle actions."""

    class Move:
        STRING = "Move to"

        def __init__(self, x, y, trace):
            self.x = x
            self.y = y
            self.trace = trace

        
        def execute(self, ctx, vec):
            print("%s (%d, %d)" % (self.STRING, self.x, self.y))

            if self.trace:
                ctx.line_to(self.x, self.y)
            else:
                ctx.move_to(self.x, self.y)


    class Forward:
        STRING = "Move forward by"

        def __init__(self, step, trace):
            self.step = step
            self.trace = trace


        def execute(self, ctx, vec):
            print("%s %d" % (self.STRING, self.step))

            dx = int(vec.x * self.step)
            dy = int(vec.y * self.step)

            if self.trace:
                ctx.rel_line_to(dx, dy)
            else:
                ctx.rel_move_to(dx, dy)


    class Backward(Forward):
        STRING = "Move backward by"

        def __init__(self, step, trace):
            super(Turtle.Backward, self).__init__(-step, trace)


    class Rotate:
        STRING = "Rotate by"

        def __init__(self, angle):
            self.angle = angle


        def execute(self, ctx, vec):
            print("%s %d" % (self.STRING, self.angle))

            vec.rotate(self.angle)


    class Center(Move):
        STRING = "Go to center"

        def __init__(self):
            super(Turtle.Center, self).__init__(WIDTH / 2, HEIGHT / 2, False)


    ACTIONS = [Move, Forward, Backward, Rotate, Center]


class TurtleWindow(Gtk.Window):

    def __init__(self, app):
        super(TurtleWindow, self).__init__(title="Turtle", application=app)

        # Grid holding all components.
        grid = Gtk.Grid()
        grid.set_column_spacing(10)

        # Should moving also left a trace?
        self.trace = Gtk.Switch(active=True)

        label = Gtk.Label(label="Trace")

        grid.attach(label, 0, 0, 1, 1)
        grid.attach(self.trace, 0, 1, 1, 1)

        # Operation picking.        
        lmodel = Gtk.ListStore(str)
        for item in Turtle.ACTIONS:
            lmodel.append([item.STRING])

        cell = Gtk.CellRendererText() # I have no idea what I'm doing :<
        
        self.operation = Gtk.ComboBox(model=lmodel)
        self.operation.pack_start(cell, False)
        self.operation.add_attribute(cell, 'text', 0)
        self.operation.set_active(0)

        self.operation.connect('changed', self.on_operation_change)

        label = Gtk.Label(label="Operation")

        grid.attach(label, 1, 0, 1, 1)
        grid.attach(self.operation, 1, 1, 1, 1)

        # Operation parameters.
        xadj = Gtk.Adjustment(0, -1000, 1000, 1, 0, 0)
        self.xarg = Gtk.SpinButton(adjustment=xadj, climb_rate=1, digits=0)

        yadj = Gtk.Adjustment(0, -1000, 1000, 1, 0, 0)
        self.yarg = Gtk.SpinButton(adjustment=yadj, climb_rate=1, digits=0)

        self.xlabel = Gtk.Label("X")
        self.ylabel = Gtk.Label("Y")

        grid.attach(self.xlabel, 2, 0, 1, 1)
        grid.attach(self.xarg, 2, 1, 1, 1)
        grid.attach(self.ylabel, 3, 0, 1, 1)
        grid.attach(self.yarg, 3, 1, 1, 1)

        # Something to send a request and remove previous one.
        self.execute = Gtk.Button(label="Execute")
        self.undo = Gtk.Button(label="Undo")

        self.execute.connect('clicked', self.on_execute_clicked)
        self.undo.connect('clicked', self.on_undo_clicked)

        grid.attach(self.undo, 4, 0, 1, 1)
        grid.attach(self.execute, 4, 1, 1, 1)

        # # And of course, a place for turtle to show up.
        self.darea = Gtk.DrawingArea()

        # I have no idea how this works but it does.
        swindow = Gtk.ScrolledWindow()
        swindow.set_size_request(WIDTH, HEIGHT)
        swindow.set_hexpand(True)
        swindow.set_vexpand(True)
        swindow.add(self.darea)
        grid.attach(swindow, 0, 2, 5, 1)

        self.darea.connect('draw', self.run_turtle)
        self.actions = [Turtle.Center()]

        # Finally, just apply a grid to window.
        self.add(grid)


    def on_operation_change(self, combo):
        op = Turtle.ACTIONS[combo.get_active()]

        # Depending on choosen action, set appropriate labels.
        if op is Turtle.Center:
            self.xlabel.set_text("")
            self.ylabel.set_text("")
            self.xarg.set_editable(False)
            self.yarg.set_editable(False)
        elif op is Turtle.Rotate:
            self.xlabel.set_text("Angle")
            self.ylabel.set_text("")
            self.xarg.set_editable(True)
            self.yarg.set_editable(False)
        elif op is Turtle.Forward or op is Turtle.Backward:
            self.xlabel.set_text("Step")
            self.ylabel.set_text("")
            self.xarg.set_editable(True)
            self.yarg.set_editable(False)
        else:
            self.xlabel.set_text("X")
            self.ylabel.set_text("Y") 
            self.xarg.set_editable(True)
            self.yarg.set_editable(True)


    def on_undo_clicked(self, button):
        # Pop only if we have more than one action (which is "center").
        if len(self.actions) > 1:
            self.actions.pop()

        self.darea.queue_draw()


    def on_execute_clicked(self, button):
        op = Turtle.ACTIONS[self.operation.get_active()]

        x = int(self.xarg.get_text())
        y = int(self.yarg.get_text())
        trace = self.trace.get_active()

        if op is Turtle.Move:
            self.actions.append(Turtle.Move(x, y, trace))
        elif op is Turtle.Forward:
            self.actions.append(Turtle.Forward(x, trace))
        elif op is Turtle.Backward:
            self.actions.append(Turtle.Backward(x, trace))
        elif op is Turtle.Rotate:
            self.actions.append(Turtle.Rotate(x))
        else:
            self.actions.append(Turtle.Center())

        self.darea.queue_draw()


    def run_turtle(self, darea, ctx):
        R = 5.0
        vec = Vector(0.0, -1.0)
        
        # Draw turtle trace.
        for action in self.actions:
            action.execute(ctx, vec)

        (x, y) = ctx.get_current_point()
        ctx.stroke()

        # Draw the turtle (which is just a circle and direction).
        ctx.save()
        ctx.arc(x, y, R, 0.0, 2 * math.pi)
        ctx.close_path()
        ctx.set_source_rgb(0.0, 0.8, 0.0)
        ctx.fill_preserve()
        ctx.restore()
        ctx.stroke()

        ctx.move_to(x, y)
        ctx.line_to(int(x + vec.x * R), int(y + vec.y * R))
        ctx.stroke()


class TurtleApplication(Gtk.Application):

    def do_activate(self):
        window = TurtleWindow(self)
        # window.set_title("Turtle")
        window.show_all()       


app = TurtleApplication()
sys.exit(app.run(sys.argv))
