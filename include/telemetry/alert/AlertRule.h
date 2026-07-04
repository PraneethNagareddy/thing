#ifndef THING_ALERTRULE_H
#define THING_ALERTRULE_H

#include <optional>
#include <functional>
#include <utility>
#include <concepts>
#include <memory>

#include "Alert.h"
#include "telemetry/Reading.h"

using namespace telemetry::alert;

namespace telemetry::alert {
class AlertRule {
     private:
         struct IAlertRuleConcept {
             virtual ~IAlertRuleConcept() = default;
             virtual std::optional<Alert> evaluate(const BaseReading& reading) const = 0;
         };

         template <typename M>
         struct AlertRuleModel : public IAlertRuleConcept {
             using EvaluatorFunc = std::function<std::optional<Alert>(const M&)>;
             using ApplicabilityFunc = std::function<bool(const M&)>;

             EvaluatorFunc evaluator;
             ApplicabilityFunc applicability;

             AlertRuleModel(EvaluatorFunc eval)
                 : evaluator(std::move(eval)) {}

             std::optional<Alert> evaluate(const BaseReading& reading) const override {
                 if (const M* specific = dynamic_cast<const M*>(&reading)) {
                     return evaluator ? evaluator(*specific) : std::nullopt;
                 }
                 return std::nullopt;
             }
         };

         std::shared_ptr<const IAlertRuleConcept> concept_;

         explicit AlertRule(std::shared_ptr<const IAlertRuleConcept> concept_)
             : concept_(std::move(concept_)) {}

     public:
         AlertRule() = default;

         [[nodiscard]] std::optional<Alert> evaluate(const BaseReading& reading) const {
             return concept_ ? concept_->evaluate(reading) : std::nullopt;
         }

         /**
          * The Builder remains templated on M to provide a type-safe API
          * during the construction of the rule.
          */
         template <typename M>
         requires std::derived_from<M, ::telemetry::BaseReading>
         class Builder {
         private:
             typename AlertRuleModel<M>::EvaluatorFunc evaluator_;

         public:
             Builder& with_evaluator(typename AlertRuleModel<M>::EvaluatorFunc func) {
                 evaluator_ = std::move(func);
                 return *this;
             }

             AlertRule build() {
                 return AlertRule(std::make_shared<AlertRuleModel<M>>(
                     std::move(evaluator_)));
             }
         };
     };
}

#endif // THING_ALERTRULE_H