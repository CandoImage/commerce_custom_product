<?php
/**
 * @file
 * Contains all pane method callbacks.
 */

/**
 * Commerce checkout method callback for CALLBACK_settings_form().
 */
function commerce_custom_product_customizing_pane_settings_form($checkout_pane) {
  $form = array(
    'commerce_custom_product_customizing_pane_description' => array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => variable_get('commerce_custom_product_customizing_pane_description', ''),
    ),
    'commerce_custom_product_customizing_pane_update_button' => array(
      '#type' => 'checkbox',
      '#title' => t('Add update button'),
      '#description' => t('Add update button on the checkout pane.'),
      '#default_value' => variable_get('commerce_custom_product_customizing_pane_update_button', TRUE),
    ),
    'commerce_custom_product_customizing_skip' => array(
      '#type' => 'checkbox',
      '#title' => t('Skip customization if no customizable products are available'),
      '#description' => t('Continues to the next checkout step if no customizable products are available.'),
      '#default_value' => variable_get('commerce_custom_product_customizing_skip', TRUE),
    ),
  );

  // Build an options array of Views available for the cart contents pane.
  $options = array();

  // Generate an option list from all user defined and module defined Views,
  // excluding the core Commerce cart block / form and order listing Views.
  $exclude = array(
    'commerce_cart_block',
    'commerce_cart_form',
    'commerce_orders',
    'commerce_user_orders',
  );
  $default = variable_get('commerce_custom_product_contents_pane_view', 'commerce_customizing_summary|default');

  foreach (views_get_all_views() as $view_id => $view_value) {
    // Only include line item Views, including a View that may be excluded but
    // has already been set to be the selected View some other way. The list of
    // excluded Views was added in as of Commerce 1.5, so we want to preserve
    // existing selections much like we do for Price fields with currency select
    // lists whose currency may have been disabled since the price was entered.
    if ($view_value->base_table == 'commerce_order') {
      foreach ($view_value->display as $display_id => $display_value) {
        $key = $view_id . '|' . $display_id;

        if (!in_array($view_id, $exclude) || $key == $default) {
          $options[$view_id][$view_id . '|' . $display_id] = $display_value->display_title;
        }
      }
    }
  }

  $form['commerce_custom_product_contents_pane_view'] = array(
    '#type' => 'select',
    '#title' => t('Customizing Review View'),
    '#description' => t('Specify the View to use in review. It should not include line item summary links or any Views form elements (e.g. quantity textfiedls or delete buttons).') . '<br />' . t('You are not allowed to use any default Views defined by core Commerce modules except the Customizing summary View.'),
    '#options' => $options,
    '#default_value' => $default,
  );

  return $form;
}

/**
 * Commerce checkout method callback for CALLBACK_checkout_form().
 */
function commerce_custom_product_customizing_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $form = array();

  $extra_line_item_data = commerce_custom_product_order_get_existing_extra_line_items($order);

  // Add description.
  $description = variable_get('commerce_custom_product_customizing_pane_description', '');
  if ($description) {
    $form['description'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="postauto-commerce-customizing-pane-description">',
      '#markup' => check_plain($description),
      '#suffix' => '</div>',
      '#weight' => -10,
    );
  }
  $form['extra_line_items']['#tree'] = TRUE;
  foreach ($extra_line_item_data as $parent_line_item_id => $data) {
    $form['extra_line_items'][$parent_line_item_id] = array(
      '#type' => 'fieldset',
      '#title' => $data['parent_line_item']->line_item_label,
    );

    foreach ($data['extra_line_items'] as $i => $extra_line_item) {
      if (!$i && ($customizing_settings = commerce_custom_product_get_customizing_settings_from_line_item($extra_line_item))) {
        if (!empty($customizing_settings['label'])) {
          $form['extra_line_items'][$parent_line_item_id]['#title'] .= '<p class="customizing-label">' . check_plain($customizing_settings['label']) . '</p>';
        }
        if (!empty($customizing_settings['description'])) {
          $form['extra_line_items'][$parent_line_item_id]['#description'] = filter_xss($customizing_settings['description']);
        }
      }

      $parents = array(
        'extra_line_items',
        $parent_line_item_id,
        'line_items',
        $i,
      );
      $extra_line_item_form_element = &$form['extra_line_items'][$parent_line_item_id]['line_items'][$i];
      $extra_line_item_form_element = commerce_custom_product_extra_line_item_form_element($extra_line_item, $form_state, $parents);
      $extra_line_item_form_element['#weight'] = $i;
      $extra_line_item_form_element['line_item_id'] = array(
        '#type' => 'value',
        '#value' => $extra_line_item->line_item_id,
      );
    }
  }
  if (variable_get('commerce_custom_product_customizing_pane_update_button', TRUE)) {
    $form['update'] = array(
      '#type' => 'submit',
      '#value' => t('Update customization'),
      '#submit' => array('commerce_custom_product_customizing_pane_checkout_form_update'),
    );
    // Ensure this file is available.
    form_load_include($form_state, 'inc', 'commerce_custom_product', 'commerce_custom_product.panes');
  }

  return $form;
}

/**
 * Commerce checkout method callback for CALLBACK_checkout_validate().
 */
function commerce_custom_product_customizing_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  return TRUE;
}

/**
 * Submit handler for the update button.
 */
function commerce_custom_product_customizing_pane_checkout_form_update($form, &$form_state) {
  commerce_custom_product_customizing_pane_checkout_form_submit($form, $form_state, commerce_checkout_pane_load('customizing_pane'), $form_state['order']);
  $form_state['rebuild'] = TRUE;
}

/**
 * Commerce checkout method callback for CALLBACK_checkout_form_submit().
 */
function commerce_custom_product_customizing_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  foreach ($form_state['values']['extra_line_items'] as $parent_line_item_id => $extra_line_items) {
    foreach ($extra_line_items['line_items'] as $i => $line_item_field_values) {
      $line_item_id = $line_item_field_values['line_item_id'];
      $line_item_form_element = $form[$checkout_pane['pane_id']]['extra_line_items'][$parent_line_item_id]['line_items'][$i];
      // Remove line item field values the user didn't have access to modify.
      foreach ($line_item_field_values as $field_name => $value) {
        // Note that we're checking the Commerce Cart settings that we
        // inserted into this form element array back when we built the form.
        // This means a module wanting to alter a line item field widget to be
        // available must update both its form element's #access value and the
        // field_access value of the #commerce_cart_settings array.
        if (empty($line_item_form_element[$field_name]['#commerce_cart_settings']['field_access'])) {
          unset($line_item_field_values[$field_name]);
        }
      }
      // Unset the line item field values array if it is now empty.
      if (!empty($line_item_field_values)) {
        $line_item = commerce_line_item_load($line_item_id);
        // Add field data to the line item.
        field_attach_submit('commerce_line_item', $line_item, $line_item_form_element, $form_state);
        commerce_line_item_save($line_item);
      }
    }
  }
}

/**
 * The customizing review pane.
 */
function commerce_custom_product_customizing_pane_review($form, $form_state, $checkout_pane, $order) {
  drupal_add_css(drupal_get_path('module', 'commerce_custom_product') . '/theme/commerce_custom_product.theme.css');

  // Extract the View and display keys from the cart contents pane setting.
  list($view_id, $display_id) = explode('|', variable_get('commerce_custom_product_contents_pane_view', 'commerce_customizing_summary|default'));

  return commerce_embed_view($view_id, $display_id, array($order->order_id));
}
