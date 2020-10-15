#ifndef READ_CHAIN_HPP
#define READ_CHAIN_HPP

#include "TROOT.h"
#include "TChain.h"
#include "TString.h"
#include "TError.h"

#include "units/utils.hpp"

class ReadChain
{

public:
    ReadChain(std::string const& name, std::string const& path) :
        _chain{TChain(name.c_str())},
        _path{path},
        _begFile{0},
        _endFile{0},
        _beg{0},
        _end{0},
        _intVar{0},
        _floatVar{0},
        _intSum{0},
        _floatSum{0}
    {}

    auto GetIntSum() const { return _intSum; }
    auto GetFloatSum() const { return _floatSum; }

    void SetFileRange(int in, int fin)
    {
        _begFile = in;
        _endFile = fin;
    }

    void Initialize()
    {
        for (auto i = _begFile; i < _endFile; ++i) {
            auto file = _path + "/file%i.root";
            _chain.Add(TString::Format(file.c_str(), i));
        }
        auto entries = _chain.GetEntries();
        _chain.SetBranchAddress("IntVar", &_intVar);
        _chain.SetBranchAddress("FloatVar", &_floatVar);
        _end = entries;
        AgentRange<cor::Group>(_beg, _end);
    }

    void ProcessChain()
    {
        for (Long64_t entry = _beg; entry < _end; ++entry) {
            if (_chain.GetEntry(entry) <= 0) {
                Error("readFiles", "Failed to read entry %lld", entry);
                return;
            }
            _intSum += _intVar;
            _floatSum += _floatVar;
        }
    }

private:
    TChain _chain;
    std::string _path;
    std::size_t _begFile, _endFile;
    std::size_t _beg, _end;
    Int_t _intVar;
    Float_t _floatVar;
    Int_t _intSum;
    Float_t _floatSum;

};

#endif
