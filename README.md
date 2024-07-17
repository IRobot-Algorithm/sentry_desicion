# Sentry Desicion

## 介绍

本仓库为2024哨兵决策仓库

main为双头构型，spare-sentry为单头构型

## 行为树节点

- Sequence

  顺序执行，一个节点返回`FAILURE`，则返回`FAILURE`

- Fallback

  顺序执行，一个节点返回`SUCCESS`，则返回`SUCCESS`

- PipelineSequence

  顺序执行，但一个节点在`running`时，前面节点将被一直触发，只要一个返回`FAILURE`，则返回`FAILURE`

  如果行为树有不是瞬时触发的节点，可以使用，不然与Sequence没有区别 （可以添加Recovery使瞬时触发的节点改为延时触发，比如连续买弹）

- ReactiveFallback

  一个节点在`running`时，Fallback会一直执行这个节点，而ReactiveFallback会在等待这个节点`running`的同时检查前面的节点是否返回`SUCCESS`

- Recovery

  第一个必须完成，如果第一个返回失败，则执行后面的节点，后面的节点可以设置为帮助第一个节点完成，然后继续执行第一个节点，直到完成或超出重复尝试次数

  - number_of_retries : 重复尝试次数

  ```xml
  <root main_tree_to_execute="MainTree">
      <BehaviorTree ID="MainTree">
          <RecoveryNode number_of_retries="1">
              <ComputePathToPose/>
              <ClearLocalCostmap/>
          </RecoveryNode>
      </BehaviorTree>
  </root>
  ```

- RoundRobin

  一直执行直到返回成功

  ```xml
  <root main_tree_to_execute="MainTree">
      <BehaviorTree ID="MainTree">
          <RoundRobin>
              <Action_A/>
              <Action_B/>
              <Action_C/>
          </RoundRobin>
      </BehaviorTree>
  </root>
  ```

