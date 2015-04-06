/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef SortInvocations_hpp_
#define SortInvocations_hpp_

#include <set>

#include "fakeit/Invocation.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/Sequence.hpp"

namespace fakeit {

	struct InvocationUtils {

		static void sortByInvocationOrder(std::unordered_set<Invocation *> &ivocations, std::vector<Invocation *> &result) {
			auto comparator = [](Invocation *a, Invocation *b) -> bool {
				return a->getOrdinal() < b->getOrdinal();
			};
			std::set<Invocation *, bool (*)(Invocation *a, Invocation *b)> sortedIvocations(comparator);
			for (auto i : ivocations)
				sortedIvocations.insert(i);

			for (auto i : sortedIvocations)
				result.push_back(i);
		}

        static void collectActualInvocations(std::unordered_set<Invocation *> &actualInvocations,
            std::set<ActualInvocationsSource *> &invocationSources) {
            for (auto source : invocationSources) {
                source->getActualInvocations(actualInvocations);
            }
        }

        static void selectNonVerifiedInvocations(std::unordered_set<Invocation *> &actualInvocations, std::unordered_set<Invocation *> &into) {
            for (auto invocation : actualInvocations) {
                if (!invocation->isVerified()) {
                    into.insert(invocation);
                }
            }
        }

        static void collectInvocationSources(std::set<ActualInvocationsSource *> &) {
        }

        template<typename ... list>
        static void collectInvocationSources(std::set<ActualInvocationsSource *> &into, const ActualInvocationsSource &mock,
            const list &... tail) {
            into.insert(const_cast<ActualInvocationsSource *>(&mock));
            collectInvocationSources(into, tail...);
        }

        static	void collectSequences(std::vector<Sequence*>&) {
        }

        template<typename ... list>
        static void collectSequences(std::vector<Sequence*>& vec, const Sequence& sequence, const list&... tail) {
            vec.push_back(&const_cast<Sequence&>(sequence));
            collectSequences(vec, tail...);
        }

        static void collectInvolvedMocks(std::vector<Sequence *> &allSequences, std::set<ActualInvocationsSource *> &invlovedMocks) {
            for (auto sequence : allSequences) {
                sequence->getInvolvedMocks(invlovedMocks);
            }
        }

    };



}

#endif // SortInvocations_hpp_
