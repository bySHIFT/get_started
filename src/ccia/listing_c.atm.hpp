#pragma once
#include "listing_c.receiver.hpp"
#include "listing_c.dispatcher.hpp"
#include "listing_c.template_dispatcher.hpp"
#include "listing_c.entity.hpp"

class atm
{
public:
  atm(messaging::sender bank_
    , messaging::sender interface_hardware_)
    : bank(bank_)
    , interface_hardware(interface_hardware_) {}

  void done() {
    get_sender().send(messaging::close_queue());
  }

  void run() try {
    state = &atm::waiting_for_card;

    while (true) (this->*state)();
  } catch(const messaging::close_queue&) {}

  messaging::sender get_sender() {
    return (messaging::sender)incoming;
  }

private:
  messaging::receiver incoming;
  messaging::sender bank;
  messaging::sender interface_hardware;
  void (atm::*state)();
  std::string account;
  unsigned withdrawal_amount;
  std::string pin;

  void process_withdrawal() {
    incoming.wait()
    .handle<withdraw_ok>([&](const withdraw_ok& msg) {
      interface_hardware.send(issue_money(withdrawal_amount));
      bank.send(withdrawal_processed(account, withdrawal_amount));
      state = &atm::done_processing; })
    .handle<withdraw_denied>([&](const withdraw_denied& msg) {
      interface_hardware.send(display_insufficient_funds());
      state = &atm::done_processing; })
    .handle<cancel_pressed>([&](const cancel_pressed& msg) {
      bank.send(cancel_withdrawal(account, withdrawal_amount));
      interface_hardware.send(display_withdrawal_cancelled());
      state = &atm::done_processing;});
  }

  void process_balance() {
    incoming.wait()
    .handle<balance>([&](const balance& msg) {
      interface_hardware.send(display_balance(msg.amount));
      state = &atm::wait_for_action; })
    .handle<cancel_pressed>([&](const cancel_pressed& msg) {
      state = &atm::done_processing; });
  }

  void wait_for_action() {
    interface_hardware.send(display_withdrawal_options());
    incoming.wait()
    .handle<withdraw_pressed>([&](const withdraw_pressed& msg) {
      withdrawal_amount = msg.amount;
      bank.send(withdraw(account, msg.amount, (messaging::sender)incoming));
      state = &atm::process_withdrawal; })
    .handle<balance_pressed>([&](const balance_pressed& msg) {
      bank.send(get_balance(account, (messaging::sender)incoming));
      state = &atm::process_balance; })
    .handle<cancel_pressed>([&](const cancel_pressed& msg) {
      state = &atm::done_processing; });
  }

  void verifying_pin() {
    incoming.wait()
    .handle<pin_verified>([&](const pin_verified& msg) {
      state = &atm::wait_for_action; })
    .handle<pin_incorrect>([&](const pin_incorrect& msg) {
      interface_hardware.send(display_pin_incorrect_message());
      state = &atm::done_processing; })
    .handle<cancel_pressed>([&](const cancel_pressed& msg) {
      state = &atm::done_processing; });
  }

  void getting_pin() {
    incoming.wait()
    .handle<digit_pressed>([&](const digit_pressed& msg) {
      unsigned const pin_length = 4;
      pin += msg.digit;
      if (pin.length() == pin_length) {
          bank.send(verify_pin(account, pin, (messaging::sender)incoming));
          state = &atm::verifying_pin;
      }})
    .handle<clear_last_pressed>([&](const clear_last_pressed& msg) {
      if (!pin.empty()) pin.pop_back(); })
    .handle<cancel_pressed>([&](const cancel_pressed& msg) {
      state = &atm::done_processing; });
  }

  void waiting_for_card() {
    interface_hardware.send(display_enter_card());
    incoming.wait()
    .handle<card_inserted>([&](const card_inserted& msg) {
      account = msg.account;
      pin = "";
      interface_hardware.send(display_enter_pin());
      state = &atm::getting_pin; });
  }

  void done_processing() {
    interface_hardware.send(eject_card());
    state = &atm::waiting_for_card;
  }

  atm(const atm&) = delete;
  atm& operator=(const atm&) = delete;
};
