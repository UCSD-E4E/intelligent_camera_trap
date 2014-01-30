import TIME_currentTime as ct

class FiniteStateMachine:
    def __init__(self):
        self.initialState = None
        self.stateFunctions = {}

    def add_state(self, name, stateFunction):
        self.stateFunctions[name] = stateFunction

    def init(self, initialState):
        self.initialState = initialState
        
    def run(self):

        stateFunction = self.stateFunctions[self.initialState]
        print "Initial State: %s" %self.initialState

        oldState = self.initialState
        while True:
            newState = stateFunction()
            stateFunction = self.stateFunctions[newState]
            t = ct.getCurrentTime()
            for a in t :
                a = str(a)
            print "%s" %newState
            print "Time: %s" %(t["YEAR"]+"-"+t["MONTH"]+"-"+t["MONTHDAY"]+" "+ t["HOUR"]+":"+t["MINUTE"]+"."+t["SEC"]+" "+t["WEEKDAY"])
            oldState = newState

