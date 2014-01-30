import FSM_Class as FSM
import FSM_stateFunction as FSM_STF

if __name__ == '__main__':
    stm = FSM.FiniteStateMachine()
             
    stm.add_state(FSM_STF.STATE_INIT        ,FSM_STF.SF_INIT)
    stm.add_state(FSM_STF.STATE_PHONE_HOME  ,FSM_STF.SF_PHONE_HOME)
    stm.add_state(FSM_STF.STATE_SPAWN_VISION,FSM_STF.SF_SPAWN_VISION)
    stm.add_state(FSM_STF.STATE_LOGIC       ,FSM_STF.SF_LOGIC)
    stm.add_state(FSM_STF.STATE_SHUT_DOWN   ,FSM_STF.SF_SHUT_DOWN)           
    
    stm.init(FSM_STF.STATE_INIT)
    stm.run()
