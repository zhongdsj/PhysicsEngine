# pragma once
# include <atomic>
# include <functional>
# include <CommonMacro.h>
# include <CommonExport.h>

namespace ZDSJ
{
		// 类型萃取工具，用于获取可调用对象的参数类型
		template<typename T>
		struct function_traits;

		// 普通函数
		template<typename Ret, typename... Args>
		struct function_traits<Ret(Args...)> {
			using return_type = Ret;
			using argument_types = std::tuple<Args...>;
		};

		// 函数指针
		template<typename Ret, typename... Args>
		struct function_traits<Ret(*)(Args...)> : function_traits<Ret(Args...)> {};

		// 函数对象和lambda
		template<typename F>
		struct function_traits {
		private:
			using callable_t = decltype(&F::operator());
		public:
			using return_type = typename function_traits<callable_t>::return_type;
			using argument_types = typename function_traits<callable_t>::argument_types;
		};

		// 成员函数指针（处理const限定）
		template<typename C, typename Ret, typename... Args>
		struct function_traits<Ret(C::*)(Args...) const> : function_traits<Ret(Args...)> {};

		class Common_Api Slot {
		public:
			template <class Fn>
			explicit Slot(Fn&& function, const std::string& description = std::string("未添加描述")): m_description(description)
			{
				using traits = function_traits<std::decay_t<Fn>>;
				using args_tuple = typename traits::argument_types;
				// 封装可调用对象和参数类型信息
				this->m_slot = ([func = std::forward<Fn>(function)](void* row) {
					args_tuple* args = static_cast<args_tuple*>(row);
#ifdef Less_Cxx17
					Slot::apply(func, *args);
#else
					std::apply(func, *args);
#endif
				});
			}

			template <class... Args>
			void emit(Args&&... args) {
				// 创建参数元组并传递地址
				auto args_tuple = std::make_tuple(std::forward<Args>(args)...);
				this->m_slot(&args_tuple);
			}

			bool useful() const
			{
				return this->m_useful.load();
			}

			void useful(const bool _canUsed)
			{
				this->m_useful.store(_canUsed);
			}

			const std::string& description()
			{
				return this->m_description;
			}

			~Slot() = default;

		private:
			std::function<void(void*)> m_slot;
			std::atomic_bool m_useful{ true };
			std::string m_description;

#ifdef Less_Cxx17
			// 辅助函数模板，用于展开元组
			template <typename F, typename Tuple, std::size_t... I>
			static constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>) {
				return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
			}

			// 实现类似std::apply的功能
			template <typename F, typename Tuple>
			static constexpr decltype(auto) apply(F&& f, Tuple&& t) {
				return apply_impl(
					std::forward<F>(f),
					std::forward<Tuple>(t),
					std::make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{}
				);
			}
#endif
		};
}
