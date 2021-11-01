#ifndef NAMESPACE_UTIL_HPP_
#define NAMESPACE_UTIL_HPP_

#if __cplusplus == 201703L || __cplusplus == 202002L
#define BEGIN_NAMESPACE_INTERNAL namespace tl::internal{
#define END_NAMESPACE_INTERNAL }

#define BEGIN_NAMESPACE_DETAIL namespace tl::detail{
#define END_NAMESPACE_DETAIL }
#endif
#undef BEGIN_NAMESPACE_INTERNAL
#undef END_NAMESPACE_INTERNAL
#undef BEGIN_NAMESPACE_DETAIL
#undef END_NAMESPACE_DETAIL

#define BEGIN_NAMESPACE_INTERNAL namespace tl{\
    namespace internal{
#define END_NAMESPACE_INTERNAL }\
}

#define BEGIN_NAMESPACE_DETAIL namespace tl{\
    namespace detail{
#define END_NAMESPACE_DETAIL }\
}
#endif