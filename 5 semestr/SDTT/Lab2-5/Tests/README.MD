In case if you decided to run tests on other computer, check paths in test cases

---
---

# Test Plan

**Project**: Pseudo-Random Number Generator _"Circus of Blood and Pain"_

**Version**: 0.1  
**Created**: 24/11/2022  
**Last Updated**: 24/11/2022  
**Status**: RELEASE

# Revision and Sign off

## Document History

| Version | Date       | Author | Description of Change  |
| ------- | ---------- | ------ | ---------------------- |
| 0.1     | 24/11/2022 | TA     | Draft created          |
| 0.2     | 24/11/2022 | TA     | Introduction added     |
| 0.3     | 04/12/2022 | TA     | Test Strategy added    |
| 0.4     | 05/12/2022 | TA     | Test Environment added |

## Reference Documents

> The documents used as an input to create the test plan:

| Version | Date       | Document name                                                                                                       |
| ------- | ---------- | ------------------------------------------------------------------------------------------------------------------- |
| 0.1     | 24/11/2022 | [SRS](https://github.com/raik199x/SDTT/tree/main/SRS) : Pseudo-Random Number Generator _"Circus of Blood and Pain"_ |
| 0.1     | 24/11/2022 | [SDS](https://github.com/raik199x/SDTT/tree/main/SDS) : Pseudo-Random Number Generator _"Circus of Blood and Pain"_ |

# 1 Introduction

## 1.1 Purpose

This test plan describes the testing approach we used to test the **Project**.

The document introduces:  
**Test Strategy**: rules the test will be based on and description of the testing process  
**Execution Strategy**: describes how the test will be performed and process to identify and report errors  
**Test Management**: process to handle the logistics of the test and all the events that come up during execution

## 1.2 Project Overview

### 1.2.1 Project Description

Circus of Blood and Pain will be a dealer for players who gives two players random teams.

### 1.2.2 Project Functions

When the program is opened, the user can choose the level of his (and his enemy) account, choose which paid characters to him (them) are available and get the final team for the battle.  
The visual style of the application should be as close as possible to the atmosphere of the game Darkest Dungeon, custom modifications or addons are possible.

# 1.3 Audience

Project team members themselves...
write the entire document you read;
keep the document up-to-date;
write the source code for the test cases;
run them on their own risk;
deal with all test results, including fixing bugs if found any during testing process;
are just cool lads, they deserved some biscuits and sleep.

# 2 Test Strategy

## 2.1 Test Objectives

The objective of the test is to verify that the functionality of Pseudo-Random Number Generator _"Circus of Blood and Pain"_ works according to the specifications.

The test will call essential functions from the **Project**'s sources, verify values they return and signalize if an error is detected.

The final product of the test is a standalone application that will be used to develop the stable software and help programmers to ensure that there are no critical errors in the algorithms.

## 2.2 Test Assumptions

**Key Assumptions** contain:

* UI/UX and essential algorithms do require testing but also they require different approaches, which leads to the next assumptions.
* Tests must support at least one of systems declared in [SDS](https://github.com/raik199x/SDTT/tree/main/SDS), but don't have to support them all.
* Tests must be integrated with the product's source code so it be carried out at any time during any phase of the software production cycle to detect errors as soon as it's possible.
* All the errors must be reported to programmers in the convenient text format.
* For testing algorithms, preloaded data should be used to simplify test design process.
* Testing UI/UX will rely on end user experience and reports.
* All test are written, extended and executed by the project team members.

## 2.3 Test Principles

* Functional testing will be focused on the algorithm's validation.
* Testing process will be as flexible as possible to provide the ability to extend it for the new algorithms and test cases.
* Tests will be build as a standalone application to reach understandable project structuring and boost it's build speed and minimize the amount of unused code.
* User Acceptance Test (UAT) will be focused on the UI/UX and rely on end users' reports.
* Testing environment and data will emulate the production ones as much as possible.
* Testing will be a repeatable, quantifiable, and measurable activity.
* Testing will be divided into distinct test phases, each with clearly defined objectives and goals.
* There will be entrance and exit criteria.

## 2.4 Data Approach

* The testing will use pre-loaded real data, that can and will be used in the real end-product.

## 2.5 Scope and Levels of Testing

### 2.5.1 Functional Testing

**Purpose:** to check the functions of application. The functional testing is carried out by feeding the input and validates the output from the application.

**Method:** the test will use pre-loaded data (input and expected output) to verify functions' output from different inputs.

**Test Acceptance Criteria:** pass if function's output is the same as expected, else - fail.

**Testers:** the team of developers.

**Timing:** the only requirements here is not allowing eternal cycles.

#### 2.5.1.1 Testing 'ParsingTrinket' algorithms

'ParsingTrinket' algorithms must take a formatted string `"trinket_trinket[hero_hero]"` and return one of two values:

* In that case the value `"trinket_trinket"` is expected.
* In that case the value `"hero_hero"` is expected.

#### 2.5.1.2 Testing 'GetTrinkets' algorithm

The test will receive list of all possible trinkets, select those  that are available for the selected level and begin to generate random trinkets. If the trinket is not suitable (it's level is higher than selected one) it will fail.
The test is repeatable: the same process takes 1K attempts before finally pass, if any of the attempts is failed the entire test  is failed.

### 2.5.2 User Acceptance Test (UAT)

**Purpose:** this test is focused on user interface and experience. It allows the end users to make the final review of the application.

**Method:** as an old wisdom says, there are no developer which can break the system in the way, the end users can. That's why we will grand access for a bunch of people to the entire functionality of the final Product on it's different production stages. The feedbacks and bug reports that we receive will be used for the **Test Acceptance Criteria**.

**Test Acceptance Criteria:** each users' report must bypass analysis and verification of developers to be considered as passed, else it's failed.

**Testers:** the end user volunteers.

**Timing:** no requirements.

# 3 Test Environment

For all tests:  
OS: Windows, Linux.

For the functional testing:  
Programming language: c++17.  
UI/UX framework: Qt Framework 6.4.0.  
Unit Test library: Qt Test.
