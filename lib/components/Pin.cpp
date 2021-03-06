#include "IComponent.hpp"
#include "Comp4040.hpp"

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
}

nts::IComponent *nts::Pin::getLinkedComp() const {
  return _linkedComp;
}

void nts::Pin::setState(const nts::Tristate &newState) {
  _state = newState;
}

void nts::Pin::updateState(const nts::Tristate &newState) {
  _state = newState;
  if (_linkedPin) { _linkedPin->setState(newState); }
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
  { nts::GateType::FLIPFLOPD, &nts::FlowChart::flipFlopD },
  { nts::GateType::FLIPFLOPRS, &nts::FlowChart::flipFlopRS },
  { nts::GateType::TENBITSJOHNSONDECADE, &nts::FlowChart::tenBitsJohnsonDecade },
  { nts::GateType::TWELVEBITSCOUNTER, &nts::FlowChart::twelveBitsCounter },
  { nts::GateType::EIGHTBITSSHIFTER, &nts::FlowChart::eightBitsShifter },
  { nts::GateType::DECODER, &nts::FlowChart::decoder },
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
  bool a = (*inputs)[0]->getState();
  bool b = (*inputs)[1]->getState();

  if (!gate->hasDefinedPins()) { (*outputs)[0]->updateState(nts::Tristate::UNDEFINED); }
  else { (*outputs)[0]->updateState((nts::Tristate)(bool)(!(a || b))); }
}

void nts::FlowChart::NOT(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool value = !(bool)(*inputs)[0]->getState();

  if (!gate->hasDefinedPins()) {
    (*outputs)[0]->updateState(nts::Tristate::UNDEFINED);
    return;
  }
  (*outputs)[0]->updateState((nts::Tristate)(value));
}

void nts::FlowChart::XOR(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool a = (bool)(*inputs)[0]->getState();
  bool b = (bool)(*inputs)[1]->getState();
  bool res;

  if (!gate->hasDefinedPins()) {
    (*outputs)[0]->updateState(nts::Tristate::UNDEFINED);
    return;
  }
  res = a ^ b;
  (*outputs)[0]->updateState((nts::Tristate)(res));
}

void nts::FlowChart::OR(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool a = (bool)(*inputs)[0]->getState();
  bool b = (bool)(*inputs)[1]->getState();

  if (!gate->hasDefinedPins()) {
    (*outputs)[0]->updateState(nts::Tristate::UNDEFINED);
    return;
  }
  (*outputs)[0]->updateState((nts::Tristate)(a | b));
}

void nts::FlowChart::AND(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool a = (*inputs)[0]->getState();
  bool b = (*inputs)[1]->getState();

  if (!gate->hasDefinedPins()) { (*outputs)[0]->updateState(nts::Tristate::UNDEFINED); }
  else { (*outputs)[0]->updateState((nts::Tristate)(a & b)); }
}

void nts::FlowChart::NAND(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool a = (*inputs)[0]->getState();
  bool b = (*inputs)[1]->getState();

  if (!gate->hasDefinedPins()) { (*outputs)[0]->updateState(nts::Tristate::UNDEFINED); }
  else {
    (*outputs)[0]->updateState((nts::Tristate)(bool)(!(a && b)));
  }
}

void nts::FlowChart::bitsAdder(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool a = (*inputs)[0]->getState();
  bool b = (*inputs)[1]->getState();
  bool carryIn = (*inputs)[2]->getState();

  if (!gate->hasDefinedPins()) {
    (*outputs)[0]->updateState(nts::Tristate::UNDEFINED);
    (*outputs)[1]->updateState(nts::Tristate::UNDEFINED);
    return;
  }
  (*outputs)[0]->updateState(nts::Tristate( ((a + b + carryIn) == 1 || (a + b + carryIn) == 3)) );
  (*outputs)[1]->updateState(nts::Tristate( ((a + b + carryIn) > 1) ));
}

void nts::FlowChart::flipFlopD(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  nts::Tristate reset = (*inputs)[1]->getState();
  nts::Tristate set = (*inputs)[3]->getState();
  nts::Tristate data = (*inputs)[2]->getState();

  if (!gate->hasDefinedPins()) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
      pin->updateState(nts::Tristate::UNDEFINED);
    });
    return;
  }
  //  reset
  if (reset || set) {
    (*outputs)[0]->updateState(set);
    (*outputs)[1]->updateState(reset);
    return;
  }
  //  clock rising (down -> up)
  if ((*inputs)[0]->getLinkedComp() && (*inputs)[0]->getLinkedComp()->isRising()) {
    (*outputs)[0]->updateState(data);
    (*outputs)[1]->updateState(nts::Tristate(!data));
  }
}

void nts::FlowChart::flipFlopRS(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool set = (*inputs)[0]->getState();
  bool reset = (*inputs)[1]->getState();

  if (!gate->hasDefinedPins()) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
      pin->updateState(nts::Tristate::UNDEFINED);
    });
    return;
  }
  if ((set && !reset) || (!set && reset)) {
    (*outputs)[0]->setState((nts::Tristate)reset);
    (*outputs)[1]->setState((nts::Tristate)((bool)!reset));
  } else if (!set && !reset) { // Invalid
    (*outputs)[0]->setState(nts::Tristate::TRUE);
    (*outputs)[1]->setState(nts::Tristate::TRUE);
  }
  else {
    nts::Tristate old = (*outputs)[0]->getState();
    (*outputs)[0]->setState(old == nts::Tristate::UNDEFINED ? nts::Tristate::FALSE : old);
    old = (*outputs)[1]->getState();
    (*outputs)[1]->setState(old == nts::Tristate::UNDEFINED ? nts::Tristate::FALSE : old);
  }

}

void nts::FlowChart::tenBitsJohnsonDecade(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  auto it = std::find_if(outputs->begin(), outputs->end(),
        [](nts::Pin *pin) {
          return pin->getState() == nts::Tristate::TRUE;
        });
  int val = it - outputs->begin();
  bool rising = (*inputs)[1]->getLinkedComp()->isRising();
  bool reset = (*inputs)[2]->getState();

  if (!gate->hasDefinedPins()) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
      pin->updateState(nts::Tristate::UNDEFINED);
    });
    return;
  }
  //  reset
  if (reset || val == 11) {
    val = 0;
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
        pin->updateState(nts::Tristate::FALSE);
      });
    (*outputs)[0]->updateState(nts::Tristate::TRUE);
    (*outputs)[10]->updateState(nts::Tristate::TRUE);
  }
  //  if inhibit true, change clock value so it will be restored to original state
  if ((*inputs)[0]->getState() == nts::Tristate::TRUE) {
    bool x = (*inputs)[1]->getState();
    (*inputs)[1]->updateState((nts::Tristate)(!x));
  }
  if (!reset && val != 11 && rising) {
  //  if clock is rising (down -> up), incremente counter
    (*outputs)[val]->updateState(nts::Tristate::FALSE);
    val = (val + 1) % 10;
    (*outputs)[val]->updateState(nts::Tristate::TRUE);
    //  set carry state
    (*outputs)[10]->updateState(nts::Tristate(val < 5));
  }
}

void nts::FlowChart::twelveBitsCounter(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  int round = ((nts::AComponent *)((*outputs)[0]->getLinkedComp()))->getRound();

  // init or reset value
  if ((*inputs)[1]->getState() == nts::Tristate::TRUE || round == -1) {
    std::for_each(outputs->begin(), outputs->end(),
      [](nts::Pin *item) {
        item->updateState(nts::Tristate::FALSE);
    });
  }
  if ((*inputs)[1]->getState() == nts::Tristate::TRUE) {
    ((nts::AComponent *)((*outputs)[0]->getLinkedComp()))->setRound(0);
    return;
  }

  // if clock is TRUE (montante), don't do anything
  if ((*inputs)[0]->getState() == nts::Tristate::TRUE) {
    if (round == -1) {
      ((nts::AComponent *)((*outputs)[0]->getLinkedComp()))->setRound(0);
    }
    return;
  }
  round += 1;

  size_t index = 0;
  std::for_each(outputs->begin(), outputs->end(),
    [&index, round](nts::Pin *item) {
      item->updateState(nts::Tristate((round >> index) & 1));
      index++;
    });
  ((nts::AComponent *)((*outputs)[0]->getLinkedComp()))->setRound(round);
  return;
}

void nts::FlowChart::eightBitsShifter(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();

  if (!gate->hasDefinedPins()) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
      pin->updateState(nts::Tristate::UNDEFINED);
    });
    return;
  }
  if ((*inputs)[3]->getState() == nts::Tristate::FALSE) { // if OE is false
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *output) {
      output->updateState(nts::Tristate::FALSE);
    });
  }
  if ((*inputs)[2]->getLinkedComp()->isRising()) {
    (*outputs)[9]->updateState((*outputs)[6]->getState()); // Qs
    if ((*inputs)[3]->getState() == nts::Tristate::TRUE &&
        (*inputs)[0]->getState() == nts::Tristate::TRUE) {  // OE && STROBE
          nts::Tristate value = (*inputs)[1]->getState();

          std::for_each(outputs->begin(), outputs->end(),
          [&value](nts::Pin *output) {
            output->updateState(value);
          });
      }
    }
  else {
    (*outputs)[8]->updateState((*outputs)[6]->getState()); // Q's
  }
}

void nts::FlowChart::decoder(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool a = (*inputs)[0]->getState();
  bool b = (*inputs)[1]->getState();
  bool c = (*inputs)[2]->getState();
  bool d = (*inputs)[3]->getState();
  bool inhibit = (*inputs)[4]->getState();

  if (!gate->hasDefinedPins()) {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *pin) {
      pin->updateState(nts::Tristate::UNDEFINED);
    });
    return;
  }
  if (!inhibit) {
    (*outputs)[0]->updateState(nts::Tristate((bool)(~a & ~b & ~c & ~d)));
    (*outputs)[1]->updateState(nts::Tristate((bool)(a & ~b & ~c & ~d)));
    (*outputs)[2]->updateState(nts::Tristate((bool)(~a & b & ~c & ~d)));
    (*outputs)[3]->updateState(nts::Tristate((bool)(a & b & ~c & ~d)));
    (*outputs)[4]->updateState(nts::Tristate((bool)(~a & ~b & c & ~d)));
    (*outputs)[5]->updateState(nts::Tristate((bool)(a & ~b & c & ~d)));
    (*outputs)[6]->updateState(nts::Tristate((bool)(~a & b & c & ~d)));
    (*outputs)[7]->updateState(nts::Tristate((bool)(a & b & c & ~d)));
    (*outputs)[8]->updateState(nts::Tristate((bool)(~a & ~b & ~c & d)));
    (*outputs)[9]->updateState(nts::Tristate((bool)(a & ~b & ~c & d)));
    (*outputs)[10]->updateState(nts::Tristate((bool)(~a & b & ~c & d)));
    (*outputs)[11]->updateState(nts::Tristate((bool)(a & b & ~c & d)));
    (*outputs)[12]->updateState(nts::Tristate((bool)(~a & ~b & c & d)));
    (*outputs)[13]->updateState(nts::Tristate((bool)(a & ~b & c & d)));
    (*outputs)[14]->updateState(nts::Tristate((bool)(~a & b & c & d)));
    (*outputs)[15]->updateState(nts::Tristate((bool)(a & b & c & d)));
  } else {
    std::for_each(outputs->begin(), outputs->end(),
    [](nts::Pin *outPin) {
      outPin->updateState(nts::Tristate::FALSE);
    });
  }
}

void nts::FlowChart::Exec() const {
  _gateFn[_type](this);
}
