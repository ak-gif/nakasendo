#ifndef __GROUP_META_DATA_H__
#define __GROUP_META_DATA_H__

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <mutex>
#include <BigNumbers/BigNumbers.h>
#include <ECPoint/ECPoint.h>

class GroupMetadata
{
    friend std::ostream& operator<< (std::ostream&, const GroupMetadata&);
    public:
        GroupMetadata() 
            : m_groupid(), m_proposer(), m_m(), m_n(), m_t()
            , m_participantList(), m_GroupSet(false),m_calculationType(std::string())
            , m_GroupInviteReplies(0), m_GroupSignatureReplies(0)
         { return ; }
        GroupMetadata(const std::string&, const std::string&, const int&, const int&, const int&);
        ~GroupMetadata(){return;}
        
        GroupMetadata(const GroupMetadata&);
        GroupMetadata& operator= (const GroupMetadata&);
        
        const std::string& groupid() const { return m_groupid;}
        const std::string& proposer() const { return m_proposer;}
        const int& m() const { return m_m; }
        const int& n() const { return m_n;}
        const int& t() const { return m_t;}
        
        const bool& GroupSet() const { return m_GroupSet; }
        bool& GroupSet() { return m_GroupSet;}
        
        const int& GroupInviteReplies() const { return m_GroupInviteReplies; }
        int& GroupInviteReplies() { return m_GroupInviteReplies; }
        
        const int& GroupSignatureReplies() const { return m_GroupSignatureReplies;}
        int& GroupSignatureReplies() { return m_GroupSignatureReplies;}
        
        const std::vector<std::string>& participantList () const { return m_participantList; }
        std::vector<std::string>& participantList() { return m_participantList; }
        
        void addUserToGroup(const std::string&); 
        
        bool removeUserFromparticipantList(const std::string&); 
        
        const std::string& calculationType () const { return m_calculationType;}
        std::string& calculationType () { return m_calculationType;}
        
        const std::map<std::string, std::vector<std::pair<std::string, BigNumber> > >& collatedEvals() const { return m_CollatedEvals;}
        void addCollatedEvals(const std::string&, const std::string&, const std::string&); 
        
        
        const std::map<std::string, std::vector<ECPoint> >& CollatedHiddenPolys () const { return m_CollatedHiddenPolys;}
        void addCollatedHiddenPolys(const std::string&, const std::vector<std::string>& ); 
        
        
        const std::map<std::string, std::vector<std::pair<std::string, ECPoint> > >&  CollatedHiddenEvals () const { return m_CollatedHiddenEvals;}
        void addCollatedHiddenEvals(const std::string&, const std::string&, const std::string&);
        
        const std::map<std::string, std::pair<BigNumber, ECPoint> >& CollatedVW() const { return m_CollatedVW ;}
        void addCollatedVW (const std::string&, const std::string&, const std::string&);
        
        const std::vector<std::pair<std::string,BigNumber> >& CollatedPartialSignatures() const { return m_CollatedPartialSignatures;}
        void addCollatedPartialSignautre(const std::string&, const std::string&);
        void clearSharedDataContainers();
        void clearSignatureContainers();
        void clearPreSignatureContainers();


        const bool& isLocked () const { return m_GroupLocked ;}
        void LockGroup() { std::lock_guard<std::mutex> guard(m_GroupLock); m_GroupLocked = true;}
        void unLockGroup(){ std::lock_guard<std::mutex> guard(m_GroupLock); m_GroupLocked = false;}
    private:
        
        std::string m_groupid;
        std::string m_proposer;
        int m_m;
        int m_n; 
        int m_t; 
        
        std::vector<std::string> m_participantList;
        std::string m_calculationType;
        bool m_GroupSet;
        int m_GroupInviteReplies; 

        int m_GroupSignatureReplies;
        
        std::map<std::string, std::vector<std::pair<std::string, BigNumber> > > m_CollatedEvals;
        std::map<std::string, std::vector<ECPoint> > m_CollatedHiddenPolys;
        std::map<std::string, std::vector<std::pair<std::string, ECPoint> > > m_CollatedHiddenEvals;
        std::map<std::string, std::pair<BigNumber, ECPoint> > m_CollatedVW ;
        std::vector<std::pair<std::string, BigNumber> > m_CollatedPartialSignatures; 

        std::mutex m_GroupLock;
        bool m_GroupLocked; 
        
    
};
#endif
