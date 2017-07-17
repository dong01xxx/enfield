#include "enfield/Transform/QModulefyPass.h"
#include "enfield/Transform/QModule.h"
#include "enfield/Transform/IdTable.h"
#include "enfield/Support/RTTI.h"

#include <unordered_map>

extern std::unordered_map<std::string, std::string> StdLib;

efd::QModulefyPass::QModulefyPass(QModule::sRef qmod) : mMod(qmod) {
    mUK = Pass::K_AST_PASS;
}

void efd::QModulefyPass::initImpl(bool force) {
    mMod->mTable = IdTable::Create(nullptr);
    mCurrentTable = mMod->mTable.get();
    mIncludes.clear();
}

void efd::QModulefyPass::visit(NDQasmVersion::Ref ref) {
    if (mMod->mVersion == nullptr)
        mMod->mVersion = ref;
    ref->getStatements()->apply(this);
}

void efd::QModulefyPass::visit(NDInclude::Ref ref) {
    mIncludes.insert(ref->getFilename()->getVal());
    if (mIncludes.size() == StdLib.size())
        mMod->mStdLibsParsed = true;
    ref->getInnerAST()->apply(this);
}

void efd::QModulefyPass::visit(NDDecl::Ref ref) {
    mMod->mRegDecls.push_back(ref);
    mCurrentTable->addQVar(ref->getId()->getVal(), ref);
}

void efd::QModulefyPass::visit(NDGateDecl::Ref ref) {
    mMod->mGates.push_back(ref);
    mCurrentTable->addQGate(ref->getId()->getVal(), ref);

    mMod->mIdTableMap[ref] = IdTable::Create(mCurrentTable);
    mCurrentTable = mMod->mIdTableMap[ref].get();
    for (auto& varRef : *ref->getQArgs()) {
        std::string id = dynCast<NDId>(varRef.get())->getVal();
        mCurrentTable->addQVar(id, varRef.get());
    }
    mCurrentTable = mCurrentTable->getParent();
}

void efd::QModulefyPass::visit(NDOpaque::Ref ref) {
    mMod->mGates.push_back(ref);
    mCurrentTable->addQGate(ref->getId()->getVal(), ref);

    mMod->mIdTableMap[ref] = IdTable::Create(mCurrentTable);
    mCurrentTable = mMod->mIdTableMap[ref].get();
    for (auto& varRef : *ref->getQArgs()) {
        std::string id = dynCast<NDId>(varRef.get())->getVal();
        mCurrentTable->addQVar(id, varRef.get());
    }
    mCurrentTable = mCurrentTable->getParent();
}

void efd::QModulefyPass::visit(NDQOpMeasure::Ref ref) {
    mMod->mStatements.push_back(ref);
}

void efd::QModulefyPass::visit(NDQOpReset::Ref ref) {
    mMod->mStatements.push_back(ref);
}

void efd::QModulefyPass::visit(NDQOpU::Ref ref) {
    mMod->mStatements.push_back(ref);
}

void efd::QModulefyPass::visit(NDQOpCX::Ref ref) {
    mMod->mStatements.push_back(ref);
}

void efd::QModulefyPass::visit(NDQOpBarrier::Ref ref) {
    mMod->mStatements.push_back(ref);
}

void efd::QModulefyPass::visit(NDQOpGeneric::Ref ref) {
    mMod->mStatements.push_back(ref);
}

void efd::QModulefyPass::visit(NDStmtList::Ref ref) {
    if (mMod->mStmtList == nullptr)
        mMod->mStmtList = ref;

    for (auto& child : *ref)
        child->apply(this);
}

void efd::QModulefyPass::visit(NDIfStmt::Ref ref) {
    mMod->mStatements.push_back(ref);
}

efd::QModulefyPass::uRef efd::QModulefyPass::Create(QModule::sRef qmod) {
    return uRef(new QModulefyPass(qmod));
}

