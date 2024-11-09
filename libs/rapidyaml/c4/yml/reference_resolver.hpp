#ifndef _C4_YML_REFERENCE_RESOLVER_HPP_
#define _C4_YML_REFERENCE_RESOLVER_HPP_

#include "c4/yml/tree.hpp"
#include "c4/yml/detail/stack.hpp"


namespace c4 {
namespace yml {

/** @addtogroup doc_ref_utils
 * @{
 */

/** Reusable object to resolve references/aliases in the tree. */
struct RYML_EXPORT ReferenceResolver
{
    ReferenceResolver() = default;

    /** Resolve references: for each reference, look for a matching
     * anchor, and copy its contents to the ref node.
     *
     * This method first does a full traversal of the tree to gather
     * all anchors and references in a separate collection, then it
     * goes through that collection to locate the names, which it does
     * by obeying the YAML standard diktat that "an alias node refers
     * to the most recent node in the serialization having the
     * specified anchor"
     *
     * So, depending on the number of anchor/alias nodes, this is a
     * potentially expensive operation, with a best-case linear
     * complexity (from the initial traversal).
     *
     * @todo verify sanity against anchor-ref attacks (https://en.wikipedia.org/wiki/Billion_laughs_attack )
     */
    void resolve(Tree *t_);

public:

    /** @cond dev */

    struct RefData
    {
        NodeType type;
        id_type node;
        id_type prev_anchor;
        id_type target;
        id_type parent_ref;
        id_type parent_ref_sibling;
    };

    void reset_(Tree *t_);
    void gather_anchors_and_refs_();
    void gather_anchors_and_refs__(id_type n);
    id_type count_anchors_and_refs_(id_type n);

    id_type lookup_(RefData *C4_RESTRICT ra);

public:

    Tree *C4_RESTRICT m_tree;
    /** We're using this stack purely as an array. */
    detail::stack<RefData> m_refs;

    /** @endcond */
};

/** @} */

} // namespace ryml
} // namespace c4


#endif // _C4_YML_REFERENCE_RESOLVER_HPP_
