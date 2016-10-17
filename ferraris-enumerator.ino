/*

Copyright (c) 2016, Felix Morgner
Copyright (c) 2016, Hackerspace Coredump Rapperswil
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
 * Neither the name of Coredump nor the names of its contributors may be
   used to endorse or promote products derived from this software without
   specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#include "config.h"

#include "TM1637.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <WiFiClientSecure.h>

#include <cstdint>

auto gDisplay = tm1637{D0, D1};
auto gUpdater = Ticker{};
auto gPeople = 0u;
auto gShouldPost = true;
auto gClient = WiFiClientSecure{};

std::uint8_t switches[] = {D2, D3, D4, D5, D6, D7, D9, D10};

void setup() {
  WiFi.begin(kSSID, kPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  for (auto const sw : switches) {
    pinMode(sw, INPUT_PULLUP);
    digitalWrite(sw, HIGH);
  }

  gDisplay.power(true);
  gDisplay.setBrightness(6);
  gDisplay.clear();
  gUpdater.attach(0.05f, update);

  post();
}

void loop() {
  if (gShouldPost) {
    gShouldPost = false;
    post();
  }
}

unsigned read_switches() {
  auto people = 0u;

  for (auto const sw : switches) {
    people <<= 1;
    people += !digitalRead(sw);
  }

  return people;
}

void update() {
  auto const people = read_switches();
  auto static it = 0;

  if (gPeople != people) {
    gPeople = people;

    if (gPeople) {
      gDisplay.writeInteger(gPeople);
    } else {
      gDisplay.clear();
    }

    gShouldPost = true;
  }
}

void post() {
  auto value = String("value=") + gPeople;
  auto request =
      String("PUT ") + kPath + " HTTP/1.1\r\n" + "Host: " + kHost + "\r\n" +
      "Content-type: application/x-www-form-urlencoded\r\n" +
      "Content-length: " + value.length() + "\r\n" + "\r\n" + value + "\r\n";

  if (!gClient.connect(kHost, kPort) || !gClient.print(request)) {
    gShouldPost = true;
  }
}
