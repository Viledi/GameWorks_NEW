// Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//

#include "libcef_dll/ctocpp/test/translator_test_handler_ctocpp.h"
#include "libcef_dll/ctocpp/test/translator_test_handler_child_ctocpp.h"


// VIRTUAL METHODS - Body may be edited by hand.

int CefTranslatorTestHandlerCToCpp::GetValue() {
  cef_translator_test_handler_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_value))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->get_value(_struct);

  // Return type: simple
  return _retval;
}


// CONSTRUCTOR - Do not edit by hand.

CefTranslatorTestHandlerCToCpp::CefTranslatorTestHandlerCToCpp() {
}

template<> cef_translator_test_handler_t* CefCToCpp<CefTranslatorTestHandlerCToCpp,
    CefTranslatorTestHandler, cef_translator_test_handler_t>::UnwrapDerived(
    CefWrapperType type, CefTranslatorTestHandler* c) {
  if (type == WT_TRANSLATOR_TEST_HANDLER_CHILD) {
    return reinterpret_cast<cef_translator_test_handler_t*>(
        CefTranslatorTestHandlerChildCToCpp::Unwrap(
        reinterpret_cast<CefTranslatorTestHandlerChild*>(c)));
  }
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#ifndef NDEBUG
template<> base::AtomicRefCount CefCToCpp<CefTranslatorTestHandlerCToCpp,
    CefTranslatorTestHandler, cef_translator_test_handler_t>::DebugObjCt = 0;
#endif

template<> CefWrapperType CefCToCpp<CefTranslatorTestHandlerCToCpp,
    CefTranslatorTestHandler, cef_translator_test_handler_t>::kWrapperType =
    WT_TRANSLATOR_TEST_HANDLER;
