#include "IComponent.hpp"

nts::Pin::Pin(const int &id,
              const nts::Tristate &state,
              const nts::IComponent *comp,
              const nts::Tristate &computed) {
  _id = id;
  _gate = NULL;
  _linkedComp = const_cast<IComponent *>(comp);
  _state = state;
  _linkedPin = NULL;
  _computed = computed;
}

void nts::Pin::setComp(const IComponent *newComp, const int &pin) {
  _linkedComp = const_cast<IComponent *>(newComp);
  _linkedPin = _linkedComp->getPins()[pin];
  _state = nts::Tristate::UNDEFINED;
}

nts::IComponent *nts::Pin::getLinkedComp() const {
  return _linkedComp;
}

void nts::Pin::setState(const nts::Tristate &newState) {
  _state = newState;
}

void nts::Pin::setType(const nts::pinConf &type) {
  _type = type;
}

 nts::pinConf nts::Pin::getType() const {
   return _type;
}

void nts::Pin::setComputed(const nts::Tristate &computed) {
  _computed = computed;
}

nts::Tristate nts::Pin::getComputed() const {
  return _computed;
}

nts::Tristate nts::Pin::getState() const {
  return _state;
}

int nts::Pin::getID() const {
  return _id;
}

nts::Pin *nts::Pin::getLinkedPin() const {
  return _linkedPin;
}

void nts::Pin::setGate(const nts::FlowChart *gate) {
  _gate = const_cast<nts::FlowChart *>(gate);
}

nts::FlowChart *nts::Pin::getGate() const {
  return _gate;
}

std::map<nts::GateType, gateFn_t> nts::FlowChart::_gateFn = {
  { nts::GateType::NOR, &nts::FlowChart::NOR },
  { nts::GateType::BITSADDER, &nts::FlowChart::bitsAdder },
  { nts::GateType::FLIPFLOP, &nts::FlowChart::flipFlop },
  { nts::GateType::TENBITSJOHNSONDECADE, &nts::FlowChart::tenBitsJohnsonDecade },
  { nts::GateType::TWELVEBITSCOUNTER, &nts::FlowChart::twelveBitsCounter },
  { nts::GateType::EIGHTBITSSHIFTER, &nts::FlowChart::eightBitsShifter },
  { nts::GateType::NOT, &nts::FlowChart::NOT },
  { nts::GateType::NAND, &nts::FlowChart::NAND },
  { nts::GateType::AND, &nts::FlowChart::AND },
  { nts::GateType::OR, &nts::FlowChart::OR },
  { nts::GateType::XOR, &nts::FlowChart::XOR }
 };

nts::FlowChart::FlowChart(std::vector<nts::Pin *> *inputs,
                          std::vector<nts::Pin *> *outputs,
                          const nts::GateType &type) {
  std::function<void (nts::Pin *)> assignGate =
  [this](nts::Pin *pin)->void {
    pin->setGate(this);
  };

  _inputs = inputs;
  _outputs = outputs;
  std::for_each(_inputs->begin(), _inputs->end(), assignGate);
  std::for_each(_outputs->begin(), _outputs->end(), assignGate);
  _type = type;
}

std::vector<nts::Pin *> *nts::FlowChart::getInputs() const {
  return _inputs;
}

std::vector<nts::Pin *> *nts::FlowChart::getOutputs() const {
  return _outputs;
}

nts::GateType nts::FlowChart::getType() const {
  return _type;
}

bool nts::FlowChart::hasDefinedPins() const {
  return std::find_if(_inputs->begin(), _inputs->end(),
          [](const nts::Pin *pin) {
            return pin->getState() == nts::Tristate::UNDEFINED;
          }) == _inputs->end();
}

void nts::FlowChart::NOR(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  nts::Tristate newState;

  if (!gate->hasDefinedPins()) { newState = nts::Tristate::UNDEFINED; }
  else {
    newState =
      (nts::Tristate)std::accumulate(inputs->begin(), inputs->end(), 0,
      [](int &acc, nts::Pin *pin)->int {
        return acc ^ (int)pin->getState();
      });
    }
  std::for_each(outputs->begin(), outputs->end(),
  [&newState](nts::Pin *pinOutput) {
    pinOutput->setState(newState);
    pinOutput->getLinkedPin()->setState(newState);
  });
}

void nts::FlowChart::NAND(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool value = (bool)(*inputs)[0]->getState();
  bool value2 = (bool)(*inputs)[1]->getState();

  if (!gate->hasDefinedPins()) {
    (*outputs)[0]->setState(nts::Tristate::UNDEFINED);
    return;
  }
  (*outputs)[0]->setState((nts::Tristate)(!(value && value2)));
}

void nts::FlowChart::bitsAdder(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool a = (*inputs)[0]->getState();
  bool b = (*inputs)[0]->getState();
  bool carryIn = (*inputs)[0]->getState();

  if (!gate->hasDefinedPins()) {
    (*outputs)[0]->setState(nts::Tristate::UNDEFINED);
    (*outputs)[1]->setState(nts::Tristate::UNDEFINED);
    return;
  }
  (*outputs)[0]->setState(nts::Tristate(a && b));
  (*outputs)[1]->setState(nts::Tristate((a && b) || (a && carryIn) || (b && carryIn)));
}

void nts::FlowChart::flipFlop(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  nts::Tristate data = (*inputs)[2]->getState();
  nts::Tristate last = (*outputs)[2]->getState();

  if (!gate->hasDefinedPins()) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
      pin->setState(nts::Tristate::UNDEFINED);
    });
    return;
  }
  //  reset
  if ((*inputs)[1]->getState() == nts::Tristate::TRUE) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
      pin->setState(nts::Tristate::FALSE);
    });
    return;
  }
  //  clock rising (down -> up)
  if ((*inputs)[0]->getLinkedComp()->isRising()) {
    (*outputs)[0]->setState(data);
    (*outputs)[1]->setState((int)data < 0 ? nts::Tristate::UNDEFINED : nts::Tristate(!data));
    (*outputs)[2]->setState(data);
  } else { //  clock !rising
    (*outputs)[0]->setState(last);
    (*outputs)[1]->setState((int)last < 0 ? nts::Tristate::UNDEFINED : nts::Tristate(!last));
  }
}

void nts::FlowChart::tenBitsJohnsonDecade(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();

  //  TODO  test 10 bits johnson  4017
  if (!gate->hasDefinedPins()) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
      pin->setState(nts::Tristate::UNDEFINED);
    });
    return;
  }
  //  reset
  if ((*inputs)[2]->getState() == nts::Tristate::TRUE) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
        pin->setState(nts::Tristate::FALSE);
      });
    (*outputs)[0]->setState(nts::Tristate::TRUE);
    (*outputs)[10]->setState(nts::Tristate::TRUE);
    return;
  }
  if (!(*inputs)[1]->getLinkedComp()->isRising()){
    return;
  }
//  if clock is rising (down -> up), incremente counter
  auto it = std::find_if(outputs->begin(), outputs->end(),
        [](nts::Pin *pin) {
          return pin->getState() == nts::Tristate::TRUE;
        });
  int val = it - outputs->begin();
  (*outputs)[val]->setState(nts::Tristate::FALSE);
  val = (val + 1) % 10;
  (*outputs)[val]->setState(nts::Tristate::TRUE);

  //  set carry state
  (*outputs)[11]->setState(nts::Tristate(val % 5 == 0));

  //  if inhibit true, change clock value so it will be restored to original state
  if ((*inputs)[0]->getState() == nts::Tristate::TRUE) {
    bool x = (*inputs)[1]->getState();
    (*inputs)[1]->setState((nts::Tristate)(!x));
  }
}

void nts::FlowChart::twelveBitsCounter(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();

  if (!gate->hasDefinedPins()) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
      pin->setState(nts::Tristate::UNDEFINED);
    });
    return;
  }
  //  reset
  if ((*inputs)[1]->getState() == nts::Tristate::TRUE) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
        pin->setState(nts::Tristate::FALSE);
      });
    (*outputs)[0]->setState(nts::Tristate::TRUE);
    return;
  }
  if (!(*inputs)[0]->getLinkedComp()->isRising()) {
    auto it = std::find_if(outputs->begin(), outputs->end(),
          [](nts::Pin *pin) {
            return pin->getState() == nts::Tristate::TRUE;
          });
    int val = it - outputs->begin();
    (*outputs)[val]->setState(nts::Tristate::FALSE);
    val = (val + 1) % 12;
    (*outputs)[val]->setState(nts::Tristate::TRUE);
  }
}

void nts::FlowChart::XOR(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool value = (bool)(*inputs)[0]->getState();

  if (!gate->hasDefinedPins()) {
    (*outputs)[0]->setState(nts::Tristate::UNDEFINED);
    return;
  }
  std::for_each(outputs->begin(), outputs->end(),
    [&value](nts::Pin *pin) {
      value ^= (bool)pin->getState();
    });
  (*outputs)[0]->setState((nts::Tristate)value);
}

void nts::FlowChart::NOT(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool value = (bool)(*inputs)[0]->getState();

  if (!gate->hasDefinedPins()) {
    (*outputs)[0]->setState(nts::Tristate::UNDEFINED);
    return;
  }
  (*outputs)[0]->setState((nts::Tristate)(!value));
}

void nts::FlowChart::OR(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool value = (bool)(*inputs)[0]->getState();
  bool value2 = (bool)(*inputs)[1]->getState();

  if (!gate->hasDefinedPins()) {
    (*outputs)[0]->setState(nts::Tristate::UNDEFINED);
    return;
  }
  (*outputs)[0]->setState((nts::Tristate)(value || value2));
}

void nts::FlowChart::AND(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool value = (bool)(*inputs)[0]->getState();
  bool value2 = (bool)(*inputs)[1]->getState();

  if (!gate->hasDefinedPins()) {
    (*outputs)[0]->setState(nts::Tristate::UNDEFINED);
    return;
  }
  (*outputs)[0]->setState((nts::Tristate)(value && value2));
}

void nts::FlowChart::eightBitsShifter(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();

  if (!gate->hasDefinedPins()) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
      pin->setState(nts::Tristate::UNDEFINED);
    });
    return;
  }
  if ((*inputs)[2]->getLinkedComp()->isRising()) { // TODO  wtf is HI Z


  } else {

  }

}

void nts::FlowChart::Exec() const {
  _gateFn[_type](this);
}
