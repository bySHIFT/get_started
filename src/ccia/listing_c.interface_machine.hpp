#pragma once
#include "listing_c.receiver.hpp"
#include "listing_c.dispatcher.hpp"
#include "listing_c.template_dispatcher.hpp"
#include "listing_c.entity.hpp"

#include <iostream>
#include <mutex>

class interface_machine
{
public:
  void done() { get_sender().send(messaging::close_queue()); }

  void run() try {
    while (true) {
      incoming.wait()
      .handle<issue_money>([&](const issue_money& msg) {
        std::lock_guard<std::mutex> lk(iom);
        std::cout<< "Issuing " << msg.amount << std::endl; })
      .handle<display_insufficient_funds>(\
        [&](const display_insufficient_funds& msg) {
        std::lock_guard<std::mutex> lk(iom);
        std::cout<< "Insufficient funds" << std::endl; })
      .handle<display_enter_pin>([&](const display_enter_pin& msg) {
        std::lock_guard<std::mutex> lk(iom);
        std::cout<< "Please enter your PIN (0-9)" << std::endl; })
      .handle<display_enter_card>([&](const display_enter_card& msg) {
        std::lock_guard<std::mutex> lk(iom);
        std::cout<< "Please enter your card (I)" << std::endl; })
      .handle<display_balance>([&](const display_balance& msg) {
        std::lock_guard<std::mutex> lk(iom);
        std::cout<< "The balance of your account is "
          << msg.amount<<std::endl; })
      .handle<display_withdrawal_options>(\
        [&](const display_withdrawal_options& msg) {
        std::lock_guard<std::mutex> lk(iom);
        std::cout<< "Withdraw 50? (w)" << std::endl;
        std::cout<< "Display Balance? (b)" << std::endl;
        std::cout<< "Cancel? (c)" << std::endl; })
      .handle<display_withdrawal_cancelled>(\
        [&](const display_withdrawal_cancelled& msg) {
        std::lock_guard<std::mutex> lk(iom);
        std::cout<< "Withdrawal cancelled" << std::endl; })
      .handle<display_pin_incorrect_message>(\
        [&](const display_pin_incorrect_message& msg) {
        std::lock_guard<std::mutex> lk(iom);
        std::cout<< "PIN incorrect" << std::endl; })
      .handle<eject_card>([&](const eject_card& msg) {
        std::lock_guard<std::mutex> lk(iom);
        std::cout<< "Ejecting card" << std::endl; });
    }
  } catch(const messaging::close_queue&) {}

  messaging::sender get_sender() { return (messaging::sender)incoming; }

private:
  std::mutex          iom{};
  messaging::receiver incoming {};
};
