#ifndef _VARIATION_UTILS_H
#define _VARIATION_UTILS_H

#include "htslib/sam.h"
#include "data/BaseInsertion.h"
#include "util.h"
#include "VariationMap.h"
#include "Sclip.h"
#include <stdint.h>
#include "Configuration.h"
#include "Vars.h"
#include <regex>
#include <map>

using namespace std;
//inline bool isEquals(uint8_t ch1, uint8_t ch2){
//	return ch1 == ch2;
//}
//inline bool isNotEquals(uint8_t ch1, uint8_t ch2){
//	return !(ch1 == ch2);
//}

inline bool isEquals(char ch1, char ch2){
	return ch1 == ch2;
}
inline bool isNotEquals(char ch1, char ch2){
	return !(ch1 == ch2);
}

inline bool isHasAndEquals(char ch1, robin_hood::unordered_map<int, char> &ref, int index) {
	if(ref.find(index) == ref.end()){
		return false;
    }else{
		return ref[index] == ch1;
	}
}

inline bool isHasAndEquals(int index, robin_hood::unordered_map<int, char> &ref, int index2) {
	if((ref.find(index) == ref.end()) || (ref.find(index2) == ref.end())){
		return false;
	}
	return ref[index] == ref[index2];
}

inline bool isHasAndEquals(robin_hood::unordered_map<int, char> &ref, int index1, string str, int index2) {
	if(ref.find(index1) == ref.end()){
        return false;
	}
    //if (index2 < 0) index2 = index2 + str.length();
    //return refc.equals(str.charAt(index2));
	return ref[index1] == str[index2];
}

inline bool isHasAndNotEquals(char ch1, robin_hood::unordered_map<int, char> &ref, int index) {
	if(ref.find(index) == ref.end())
        return false;
	return !(ref[index] == ch1);
}

inline bool isHasAndNotEquals(robin_hood::unordered_map<int, char> &ref, int index1, string &str, int index2) {
	if(ref.find(index1) == ref.end())
		return false;
	return !(ref[index1] == str[index2]);
}

inline bool isHasAndNotEquals(robin_hood::unordered_map<int, char> &ref, int index1, char* str, int index2) {
	if(ref.find(index1) == ref.end())
		return false;
	return !(ref[index1] == str[index2]);
}


/**
 * Adjust the insertion position if necessary
 * @param bi starting position of insert
 * @param ins insert sequence
 * @param ref map of reference bases
 * @return Tuple of (int bi, String ins, int bi)
 */
inline BaseInsertion* adjInsPos(int bi, string &ins, robin_hood::unordered_map<int, char> &ref) {
    int n = 1;
    int len = ins.length();
    while(ref[bi] == ins[ins.length() - n]) {
        n++;
        if (n > len) {
            n = 1;
        }
        bi--;
    }
    if (n > 1) {
        ins = vc_substr(ins, 1 - n) + vc_substr(ins, 0, 1 - n);
    }
    return new BaseInsertion(bi, ins, bi);
}

//inline Variation* getVariation(unordered_map<int, VariationMap* > &hash,
//                                     int start,
//                                     string descriptionString) {
//	//cout << "get variation: " << start << " ==> " << descriptionString << endl;
//	VariationMap *vmap;
//	if(hash.find(start) == hash.end()){
//		//map  = new VariationMap<string, Variation*>();
//		vmap = new VariationMap();
//		hash[start] = vmap;
//	}else{
//		vmap = hash[start];
//	}
//	Variation* variation;
//	if(vmap->variation_map.find(descriptionString) == vmap->variation_map.end()){
//		variation = new Variation();
//		//map[descriptionString] = variation;
//		vmap->variation_map.insert(unordered_map<string, Variation*>::value_type(descriptionString, variation));
//	}else{
//		variation = vmap->variation_map.at(descriptionString);
//	}
//    //VariationMap<string, Variation> map = hash[start];
//    //if (map == NULL) {
//    //    map = new VariationMap<>();
//    //    hash.put(start, map);
//    //}
//    //Variation variation = map[descriptionString];
//    //if (variation == NULL) {
//    //    variation = new Variation();
//    //    map.put(descriptionString, variation);
//    //}
//    return variation;
//}

inline Variation* getVariation(robin_hood::unordered_map<int, VariationMap* > &hash,
                                     int start,
                                     string descriptionString) {
	//cout << "get variation: " << start << " ==> " << descriptionString << endl;
	VariationMap *vmap;
	Variation* variation;
	robin_hood::unordered_map<int, VariationMap*>::iterator itr;
	if((itr = hash.find(start)) != hash.end()){
		//map  = new VariationMap<string, Variation*>();
		vmap = itr->second;
	}else{
		vmap = new VariationMap();
		hash[start] = vmap;
		variation = new Variation();
		vmap->variation_map[descriptionString] = variation;
		return variation;
	}


	robin_hood::unordered_map<string, Variation*>::iterator itr2;
	if((itr2 = vmap->variation_map.find(descriptionString)) != vmap->variation_map.end()){
		return itr2->second;
	}else{
		variation = new Variation();
		//map[descriptionString] = variation;
		//vmap->variation_map.insert(unordered_map<string, Variation*>::value_type(descriptionString, variation));
		vmap->variation_map[descriptionString] = variation;
		return variation;
	}
	//VariationMap<string, Variation> map = hash[start];
    //if (map == NULL) {
    //    map = new VariationMap<>();
    //    hash.put(start, map);
    //}
    //Variation variation = map[descriptionString];
    //if (variation == NULL) {
    //    variation = new Variation();
    //    map.put(descriptionString, variation);
    //}
}
//inline Variation* getVariation(unordered_map<int, VariationMap* > &hash,
//                                     int start,
//                                     string descriptionString) {
//	//cout << "get variation: " << start << " ==> " << descriptionString << endl;
//	VariationMap *vmap ;//= new VariationMap();
//	pair<unordered_map<int, VariationMap*>::iterator, bool> ret1 =
//		hash.insert(make_pair(start, vmap));
//	//pair<int, VariationMap*> ret1 = hash.insert(unordered_map<int, VariationMap*>::value_type(start, vmap));
//	if(ret1.second == false){
//		vmap = ret1.first->second;
//	}else{
//	}
//
//	Variation* variation = new Variation();
//	pair<unordered_map<string, Variation*>::iterator, bool>  ret2 =
//		vmap->variation_map.insert(make_pair(descriptionString, variation));
//	//pair<string, Variation*> ret2 = vmap->variation_map.insert(unordered_map<string, Variation*>::value_type(descriptionString, variation));
//	if(ret2.second == false){
//		variation = ret2.first->second;
//	}
//    return variation;
//}
inline Variation* getVariationMaybe(robin_hood::unordered_map<int, VariationMap* > &hash,
							 int start,
							 char refBase) {
	if(refBase == (char)0)
        return NULL;

    //Map<String, Variation> map = hash.get(start);
    //if (map == null) {
    //    return null;
    //}
	if(hash.find(start) == hash.end()){
		return NULL;
	}else{
		robin_hood::unordered_map<string, Variation*> vmap = hash.at(start)->variation_map;
		string s_refBase(1, refBase);
		if(vmap.find(s_refBase) == vmap.end()){
			return NULL;
		}else{
			return vmap.at(s_refBase);
		}
	}
}

inline int getReferenceLength(bam1_t *record){
	int length = 0;
	uint32_t *cigar = bam_get_cigar(record);
	for(int i = 0; i < record->core.n_cigar; ++i){
		switch(bam_cigar_op(cigar[i])){
		case 0:
		case 2:
		case 3:
		case 7:
		case 8:
			length += bam_cigar_oplen(cigar[i]);
			break;
		default: break;
		}
	}
	return length;
}
//----song----
/**
 * Correct counts for negative values
 * @param varToCorrect variant   $ref
 */
inline void correctCnt(Variation *varToCorrect) {
    if (varToCorrect->varsCount < 0)
        varToCorrect->varsCount = 0;
    if (varToCorrect->highQualityReadsCount < 0)
        varToCorrect->highQualityReadsCount = 0;
    if (varToCorrect->lowQualityReadsCount < 0)
        varToCorrect->lowQualityReadsCount = 0;
    if (varToCorrect->meanPosition < 0)
        varToCorrect->meanPosition = 0;
    if (varToCorrect->meanQuality < 0)
        varToCorrect->meanQuality = 0;
    if (varToCorrect->meanMappingQuality < 0)
        varToCorrect->meanMappingQuality = 0;
    if (varToCorrect->getDir(true) < 0)
        varToCorrect->addDir(true, -varToCorrect->getDir(true));
    if (varToCorrect->getDir(false) < 0)
        varToCorrect->addDir(false, -varToCorrect->getDir(false));
}



/**
 * Adjust the count,  If ref is not NULL, the count for reference is also adjusted.
 * Variant counts of tv are added to vref and removed from ref
 * @param varToAdd variant to add counts    $vref
 * @param variant variant    $tv
 * @param referenceVar reference variant  $ref
 */
inline void adjCnt(Variation *varToAdd, Variation *variant, Variation *referenceVar) {
    varToAdd->varsCount += variant->varsCount;
    varToAdd->extracnt += variant->varsCount;
    varToAdd->highQualityReadsCount += variant->highQualityReadsCount;
    varToAdd->lowQualityReadsCount += variant->lowQualityReadsCount;
    varToAdd->meanPosition += variant->meanPosition;
    varToAdd->meanQuality += variant->meanQuality;
    varToAdd->meanMappingQuality += variant->meanMappingQuality;
    varToAdd->numberOfMismatches += variant->numberOfMismatches;
    varToAdd->pstd = true;
    varToAdd->qstd = true;
    varToAdd->addDir(true, variant->getDir(true));
    varToAdd->addDir(false, variant->getDir(false));
	if(referenceVar == NULL){
		return;
	}
    referenceVar->varsCount -= variant->varsCount;
    referenceVar->highQualityReadsCount -= variant->highQualityReadsCount;
    referenceVar->lowQualityReadsCount -= variant->lowQualityReadsCount;
    referenceVar->meanPosition -= variant->meanPosition;
    referenceVar->meanQuality -= variant->meanQuality;
    referenceVar->meanMappingQuality -= variant->meanMappingQuality;
    referenceVar->numberOfMismatches -= variant->numberOfMismatches;
    referenceVar->subDir(true, variant->getDir(true));
    referenceVar->subDir(false, variant->getDir(false));
    correctCnt(referenceVar);
}

inline void adjCnt(Variation *varToAdd, Variation *variant) {
    varToAdd->varsCount += variant->varsCount;
    varToAdd->extracnt += variant->varsCount;
    varToAdd->highQualityReadsCount += variant->highQualityReadsCount;
    varToAdd->lowQualityReadsCount += variant->lowQualityReadsCount;
    varToAdd->meanPosition += variant->meanPosition;
    varToAdd->meanQuality += variant->meanQuality;
    varToAdd->meanMappingQuality += variant->meanMappingQuality;
    varToAdd->numberOfMismatches += variant->numberOfMismatches;
    varToAdd->pstd = true;
    varToAdd->qstd = true;
    varToAdd->addDir(true, variant->getDir(true));
    varToAdd->addDir(false, variant->getDir(false));
}



inline string joinRef(robin_hood::unordered_map<int, char> &baseToPosition, int from, int to){
    string res="";
    for(int i=from; i <= to; i++){
        res+=baseToPosition[i];
    }
    return res;
}
inline string joinRef_double(robin_hood::unordered_map<int, char> &baseToPosition, int from, double to){
    string res="";
    for(int i=from; i < to; i++){
        res+=baseToPosition[i];
    }
    return res;
}
/**
 * Counts the total count of base presence in the string
 * @param str string to count characters
 * @param chr character to seek in the string
 * @return number of characters in the string
 */
inline int count(string &str, char chr) {
    int cnt = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == chr) {
            cnt++;
        }
    }
    return cnt;
}
//------this code can be optimized--------//
inline bool islowcomplexseq(string &seq) {
        int len = seq.length();
        if (len == 0)
            return true;
        int ntcnt = 0;

        int a = count(seq, 'A');
        if (a > 0) ntcnt++;
        if (a / (double)len > 0.75)
            return true;

        int t = count(seq, 'T');
        if (t > 0) ntcnt++;
        if (t / (double)len > 0.75)
            return true;

        int g = count(seq, 'G');
        if (g > 0) ntcnt++;
        if (g / (double)len > 0.75)
            return true;

        int c = count(seq, 'C');
        if (c > 0) ntcnt++;
        if (c / (double)len > 0.75)
            return true;

        return ntcnt < 3;
}

/**
 * Find the consensus sequence in soft-clipped reads. Consensus is called if
 * the matched nucleotides are &gt;90% of all softly clipped nucleotides.
 * @param softClip soft-clipped sequences    $scv
 * @param dir not used now, will be used when adaptor option will be added
 * @return consensus sequence
 */
inline string findconseq(Sclip *softClip, int dir) {
    //if (softClip->sequence != NULL) {
	//printf("findconseq: %d - %d - %s\n", softClip->nt.size(), dir,  softClip->sequence.c_str());
	//for(auto& nve : softClip->nt){
    //    unordered_map<char, int> nv = nve.second;
	//	for(auto& ent : nv){
	//		printf("%c", ent.first);
	//	}
	//	printf("|");
	//}
	//printf("\n");
	//========================================
	if (softClip->sequence != "") {
		//printf("return for sequence != empty: %s\n", softClip->sequence.c_str());
        return softClip->sequence;
    }

    int total = 0;
    int match = 0;
    string seqq;
    bool flag = false;
    for (auto& nve : softClip->nt) {
        int positionInSclip = nve.first;
        robin_hood::unordered_map<char, int> nv = nve.second;
        int maxCount = 0; //$max
        double maxQuality = 0; //$maxq
        char chosenBase = 0; //$mnt
        int totalCount = 0; //$tt
        for (auto& ent : nv) {
            char currentBase = ent.first; //$nt
            int currentCount = ent.second; //$ncnt
            totalCount += currentCount;
			//printf("current: %c, ==> currentCount: %d, maxCount: %d, currentQuality: %f, maxQuality: %f\n", currentBase, currentCount, maxCount, softClip->seq[positionInSclip][currentBase]->meanQuality, maxQuality);
			if (currentCount > maxCount || (softClip->seq.count(positionInSclip) && softClip->seq[positionInSclip].count(currentBase)
                    && softClip->seq[positionInSclip][currentBase]->meanQuality > maxQuality)) {
				//printf("current changeto: %c, ==> currentCount: %d, maxCount: %d, currentQuality: %f, maxQuality: %f\n", currentBase, currentCount, maxCount, softClip->seq[positionInSclip][currentBase]->meanQuality, maxQuality);
				maxCount = currentCount;
                chosenBase = currentBase;
                maxQuality = softClip->seq[positionInSclip][currentBase]->meanQuality;
            }
        }
        if (positionInSclip == 3 && softClip->nt.size() >= 6 && totalCount/(double)softClip->varsCount < 0.2 && totalCount <= 2) {
			//printf("break1\n");
            break;
        }
        if ((totalCount - maxCount > 2 || maxCount <= totalCount - maxCount) && maxCount / (double)totalCount < 0.8) {
            if (flag) {
				//printf("break2\n");
                break;
            }
            flag = true;
        }
        total += totalCount;
        match += maxCount;
        if (chosenBase != 0) {
			//printf("%c\n", chosenBase);
            seqq+=chosenBase;
        }
    }
    string SEQ;
    int ntSize = softClip->nt.size();
	//printf("seq1: %s\n", seqq.c_str());
    if (total != 0
            && match / (double)total > 0.9
            && seqq.length() / 1.5 > ntSize - seqq.length()
            && (seqq.length() / (double)ntSize > 0.8
            || ntSize - seqq.length() < 10
            || seqq.length() > 25)) {
        SEQ = seqq;
    } else {
        SEQ = "";
    }

    //if (!SEQ.empty() && SEQ.length() > Configuration.SEED_2) {
	//printf("SEQ2: %s\n", SEQ.c_str());
	if (!SEQ.empty() && SEQ.length() > CONF_SEED_2) {
        bool mm1 = regex_search(SEQ, regex("^.AAAAAAA"));
        bool mm2 = regex_search(SEQ, regex("^.TTTTTTT"));
		//printf("SEQ3: %s\n", SEQ.c_str());
        if (mm1 || mm2) {
			printf("used set to true 1\n");
            softClip->used = true;
        }
        if (islowcomplexseq(SEQ)) {
			printf("used set to true 2: %s\n", SEQ.c_str());
            softClip->used = true;
        }
    }
    //-------------------------TODO:instance?
    /*
    if (!SEQ.empty() && SEQ.length() >= Configuration.ADSEED) {
        if ( dir == 3 ) { // 3'
            if (instance().adaptorForward.count(SEQ.substr(0, Configuration.ADSEED))) {
                SEQ = "";
            }
        } else if ( dir == 5 ) { // 5'
            if (instance().adaptorReverse.count(reverse(SEQ.substr(0, Configuration.ADSEED)))) {
                SEQ = "";
            }
        }
    }
    */
    softClip->sequence = SEQ;

	//if(SEQ == "TACCGATCGGA"){
	//	printf("i found it!\n");
	//}
	//printf("consequence: %s\n", SEQ.c_str());
    return SEQ;
}

inline int strandBias(int forwardCount, int reverseCount, Configuration* conf){
    if(forwardCount + reverseCount <= 12){
        return forwardCount * reverseCount >0 ?2:0;
    }

    return (forwardCount / (double)(forwardCount + reverseCount) >= conf->bias
                  && reverseCount / (double)(forwardCount + reverseCount) >= conf->bias
                  && forwardCount >= conf->minBiasReads
                  && reverseCount >= conf->minBiasReads) ? 2 : 1;

}

inline Vars* getOrPutVars(robin_hood::unordered_map<int, Vars*> &mapv, int position){
    //Vars *vars = mapv[position];
    //mapv[position] = vars;
    //return vars;
	Vars *vars;
	robin_hood::unordered_map<int, Vars*>::iterator itr;
	if((itr = mapv.find(position)) != mapv.end()){
		vars = itr->second;
	}else{
		vars = new Vars();
		mapv[position] = vars;
	}
	return vars;
}

//-----------------about htslib operation----------------------
//TODO: to be verifying
inline int getAlignmentStart(bam1_t* record){
	return record->core.pos+1;
}

//TODO: to be verifying
inline int getMateAlignmentStart(bam1_t* record){
	return record->core.mpos + 1;
}

inline int getAlignmentEnd(bam1_t* record){
	return bam_endpos(record);
}
inline int getAlignedLength(uint32_t* cigar, int n_cigar){
	int length = 0;
	for(int c_i = 0; c_i < n_cigar; c_i++){
		if(bam_cigar_op(cigar[c_i]) == BAM_CMATCH || bam_cigar_op(cigar[c_i]) == BAM_CDEL){
			length += bam_cigar_oplen(cigar[c_i]);	
		}
	}
	return length;
}
//-----------------about htslib operation----------------------
#endif
