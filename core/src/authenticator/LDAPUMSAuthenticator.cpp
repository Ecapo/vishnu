/**
 * \file LDAPUMSAuthenticator.cpp
 * \brief This file implements the LDAPUMSauthenticator
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#include <iostream>
#include "LDAPUMSAuthenticator.hpp"
#include "LDAPAuthenticator.hpp"
#include "UMSAuthenticator.hpp"

LDAPUMSAuthenticator::LDAPUMSAuthenticator(){
}

LDAPUMSAuthenticator::~LDAPUMSAuthenticator(){
}

bool
LDAPUMSAuthenticator::authenticate(UMS_Data::User& user) {
  bool authenticated = false;

  UMSAuthenticator umsAuthenticator;
  LDAPAuthenticator ldapAuthenticator;


  if (ldapAuthenticator.authenticate(user)) {
    authenticated = true;
  }
  else {
    authenticated = umsAuthenticator.authenticate(user);
  }
  return authenticated;
}

