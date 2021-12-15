use std::cell::RefCell;
use std::rc::Rc;

#[derive(Debug, PartialEq, Eq)]
pub struct TreeNode {
    pub val: i32,
    pub left: Option<Rc<RefCell<TreeNode>>>,
    pub right: Option<Rc<RefCell<TreeNode>>>,
}

impl TreeNode {
    #[inline]
    pub fn new(val: i32) -> Self {
        TreeNode {
            val,
            left: None,
            right: None,
        }
    }
}

struct Solution;

impl Solution {
    pub fn range_sum_bst(root: Option<Rc<RefCell<TreeNode>>>, low: i32, high: i32) -> i32 {
        let mut sum = 0;
        Self::range_sum_bst_helper(root, low, high, &mut sum);
        sum
    }

    fn range_sum_bst_helper(
        root: Option<Rc<RefCell<TreeNode>>>,
        low: i32,
        high: i32,
        sum: &mut i32,
    ) {
        if let Some(node) = root {
            let node = node.borrow();
            if node.val >= low && node.val <= high {
                *sum += node.val;
            }
            if node.val > low {
                Self::range_sum_bst_helper(node.left.clone(), low, high, sum);
            }
            if node.val < high {
                Self::range_sum_bst_helper(node.right.clone(), low, high, sum);
            }
        }
    }
}

fn main() {}
