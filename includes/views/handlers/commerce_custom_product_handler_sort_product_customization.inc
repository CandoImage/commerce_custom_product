<?php
/**
 * @file
 * Views integration for commerce_custom_product.
 */

/**
 * Sort handler to sort product customizations.
 */
class commerce_custom_product_handler_sort_product_customization extends views_handler_sort {

  /**
   * Called to add the sort to a query.
   *
   * Here we add the field of our relationship - not sure if there isn't a
   * better way to do this. We need this in order to have the line item id later
   * when we manually order the extra line items.
   */
  public function query() {
    $this->ensure_my_table();
    $this->field_alias = $this->query->add_field($this->table_alias, 'line_item_id', NULL);
  }

  /**
   * Sort the items right after the query.
   */
  public function post_execute(&$values) {
    $original_values = $values;
    // Load all line items.
    foreach ($values as $i => $item) {
      if (isset($item->{$this->field_alias})) {
        $commerce_line_items[$i] = commerce_line_item_load($item->{$this->field_alias});
      }
    }
    $commerce_line_items = commerce_custom_product_order_sort_existing_extra_line_items($commerce_line_items);
    // No set the values to this order.
    $values = array();
    foreach ($commerce_line_items as $i => $line_item) {
      $values[] = $original_values[$i];
    }
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // Order is fixed.
    $form['order']['#access'] = FALSE;
    $form['message'] = array(
      '#type' => 'markup',
      '#markup' => '<strong>' . t('This returns a fixed order and overwrites all other sorting orders') . '</strong>',
    );
  }

  /**
   * This sorting handler can't be exposed.
   *
   * @return bool
   */
  public function can_expose() {
    return FALSE;
  }

}
