/*
 * File:   TestMatrixFactory.h
 * Author: Pantelis Sopasakis
 *
 * Created on Jul 13, 2015, 1:03:10 PM
 */

#ifndef TESTMATRIXFACTORY_H
#define	TESTMATRIXFACTORY_H

#include <cppunit/extensions/HelperMacros.h>

#define FORBES_TEST_UTILS
#include "ForBES.h"

class TestMatrixFactory : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TestMatrixFactory);

    CPPUNIT_TEST(testMakeIdentity);
    CPPUNIT_TEST(testMakeRandomMatrix);
    CPPUNIT_TEST(testMakeSparse);
    CPPUNIT_TEST(testReadSparseFromFile);
    CPPUNIT_TEST(testSparse);
    CPPUNIT_TEST(testSparse2);
    CPPUNIT_TEST(testShallow1);
    CPPUNIT_TEST(testShallow2);
    CPPUNIT_TEST(testShallow3);
    CPPUNIT_TEST(testShallow4);
    CPPUNIT_TEST(testFailSafe);
    
    CPPUNIT_TEST_SUITE_END();

public:
    TestMatrixFactory();
    virtual ~TestMatrixFactory();
    void setUp();
    void tearDown();

private:
    void testMakeIdentity();
    void testMakeRandomMatrix();
    void testMakeSparse();
    void testReadSparseFromFile();
    void testSparse();
    void testSparse2();
    void testShallow1();
    void testShallow2();
    void testShallow3();
    void testShallow4();
    void testFailSafe();

};

#endif	/* TESTMATRIXFACTORY_H */
